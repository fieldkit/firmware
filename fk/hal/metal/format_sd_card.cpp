#include "format_sd_card.h"
#include "hal/board.h"

#if defined(__SAMD51__)

#include <SPI.h>

namespace fk {

FK_DECLARE_LOGGER("sdcard");

static constexpr const char NO_NAME_STRING[] = "NO NAME    ";
static constexpr const char FAT32_STRING[] = "FAT32   ";

bool FormatSdCard::begin() {
    SD_SPI.end();

    SD_SPI.begin();

    spi_driver_.setPort(&SD_SPI);

    if (!card_.begin(&spi_driver_, PIN_SD_CS, SD_SCK_MHZ(50))) {
        logerror("failed to open sd");
        return false;
    }

    card_size_blocks_ = card_.cardSize();
    if (card_size_blocks_ == 0) {
        logerror("invalid card size");
        return false;
    }

    card_capacity_mb_ = (card_size_blocks_ + 2047) / 2048;

    loginfo("card capacity: %" PRIu32, card_capacity_mb_);

    return true;
}

bool FormatSdCard::erase() {
    static constexpr uint32_t ERASE_SIZE = 262144L;
    uint32_t first_block = 0;
    uint32_t last_block;

    do
    {
        last_block = first_block + ERASE_SIZE - 1;
        if (last_block >= card_size_blocks_) {
            last_block = card_size_blocks_ - 1;
        }

        if (!card_.erase(first_block, last_block)) {
            logerror("erase failed");
            return false;
        }

        first_block += ERASE_SIZE;
    }
    while (first_block < card_size_blocks_);

    if (!card_.readBlock(0, cache_.data)) {
        logerror("erase readBlock failed");
        return false;
    }

    return true;
}

bool FormatSdCard::format() {
    if (card_capacity_mb_ <= 6) {
        logerror("card is too small");
        return false;
    }
    else if (card_capacity_mb_ <= 16) {
        sectors_per_cluster_ = 2;
    }
    else if (card_capacity_mb_ <= 32) {
        sectors_per_cluster_ = 4;
    }
    else if (card_capacity_mb_ <= 64) {
        sectors_per_cluster_ = 8;
    }
    else if (card_capacity_mb_ <= 128) {
        sectors_per_cluster_ = 16;
    }
    else if (card_capacity_mb_ <= 1024) {
        sectors_per_cluster_ = 32;
    }
    else if (card_capacity_mb_ <= 32768) {
        sectors_per_cluster_ = 64;
    }
    else {
        sectors_per_cluster_ = 128;
    }

    sectors_per_track_ = card_capacity_mb_ <= 256 ? 32 : 63;

    if (card_capacity_mb_ <= 16) {
        number_of_heads_ = 2;
    }
    else if (card_capacity_mb_ <= 32) {
        number_of_heads_ = 4;
    }
    else if (card_capacity_mb_ <= 128) {
        number_of_heads_ = 8;
    }
    else if (card_capacity_mb_ <= 504) {
        number_of_heads_ = 16;
    }
    else if (card_capacity_mb_ <= 1008) {
        number_of_heads_ = 32;
    }
    else if (card_capacity_mb_ <= 2016) {
        number_of_heads_ = 64;
    }
    else if (card_capacity_mb_ <= 4032) {
        number_of_heads_ = 128;
    }
    else {
        number_of_heads_ = 255;
    }

    uint32_t nc;
    relative_sector_ = BU32;
    for (data_start_ = 2 * BU32;; data_start_ += BU32) {
        nc = (card_size_blocks_ - data_start_) / sectors_per_cluster_;
        fat_size_ = (nc + 2 + 127) / 128;
        uint32_t r = relative_sector_ + 9 + 2 * fat_size_;
        if (data_start_ >= r) {
            break;
        }
    }

    // error if too few clusters in FAT32 volume
    if (nc < 65525) {
        logerror("invalid geometry: clusters");
        return false;
    }

    reserved_sectors_ = data_start_ - relative_sector_ - 2 * fat_size_;
    fat_start_ = relative_sector_ + reserved_sectors_;
    partition_size_ = nc * sectors_per_cluster_ + data_start_ - relative_sector_;

    // type depends on address of end sector
    // max CHS has lbn = 16450560 = 1024*255*63
    if ((relative_sector_ + partition_size_) <= 16450560) {
        // FAT32
        partition_type_ = 0X0B;
    }
    else {
        // FAT32 with INT 13
        partition_type_ = 0X0C;
    }
    write_mbr();
    clear_cache(true);

    fat32_boot_t *pb = &cache_.fbs32;
    pb->jump[0] = 0XEB;
    pb->jump[1] = 0X00;
    pb->jump[2] = 0X90;
    for (uint8_t i = 0; i < sizeof(pb->oemId); i++) {
        pb->oemId[i] = ' ';
    }

    pb->bytesPerSector = 512;
    pb->sectorsPerCluster = sectors_per_cluster_;
    pb->reservedSectorCount = reserved_sectors_;
    pb->fatCount = 2;
    pb->mediaType = 0XF8;
    pb->sectorsPerTrack = sectors_per_track_;
    pb->headCount = number_of_heads_;
    pb->hidddenSectors = relative_sector_;
    pb->totalSectors32 = partition_size_;
    pb->sectorsPerFat32 = fat_size_;
    pb->fat32RootCluster = 2;
    pb->fat32FSInfo = 1;
    pb->fat32BackBootBlock = 6;
    pb->driveNumber = 0X80;
    pb->bootSignature = EXTENDED_BOOT_SIG;
    pb->volumeSerialNumber = volume_serial_number();
    memcpy(pb->volumeLabel, NO_NAME_STRING, sizeof(pb->volumeLabel));
    memcpy(pb->fileSystemType, FAT32_STRING, sizeof(pb->fileSystemType));

    // write partition boot sector and backup
    if (!write_cache(relative_sector_) || !write_cache(relative_sector_ + 6)) {
        logerror("fat32 write PBS failed");
        return false;
    }
    clear_cache(true);

    // write extra boot area and backup
    if (!write_cache(relative_sector_ + 2) || !write_cache(relative_sector_ + 8)) {
        logerror("fat32 PBS extra failed");
        return false;
    }

    fat32_fsinfo_t* pf = &cache_.fsinfo;
    pf->leadSignature = FSINFO_LEAD_SIG;
    pf->structSignature = FSINFO_STRUCT_SIG;
    pf->freeCount = 0XFFFFFFFF;
    pf->nextFree = 0XFFFFFFFF;

    // write FSINFO sector and backup
    if (!write_cache(relative_sector_ + 1) || !write_cache(relative_sector_ + 7)) {
        logerror("fat32 fsinfo failed");
        return false;
    }
    clear_fat_directory(fat_start_, 2 * fat_size_ + sectors_per_cluster_);
    clear_cache(false);
    cache_.fat32[0] = 0x0FFFFFF8;
    cache_.fat32[1] = 0x0FFFFFFF;
    cache_.fat32[2] = 0x0FFFFFFF;

    // write first block of FAT and backup for reserved clusters
    if (!write_cache(fat_start_) || !write_cache(fat_start_ + fat_size_)) {
        logerror("fat32 reserve failed");
        return false;
    }

    return true;
}

bool FormatSdCard::write_mbr() {
    clear_cache(true);
    part_t* p = cache_.mbr.part;
    p->boot = 0;
    uint16_t c = lbn_to_cylinder(relative_sector_);
    if (c > 1023) {
        logerror("write_mbr invalid cylinder");
        return false;
    }

    p->beginCylinderHigh = c >> 8;
    p->beginCylinderLow = c & 0XFF;
    p->beginHead = lbn_to_head(relative_sector_);
    p->beginSector = lbn_to_sector(relative_sector_);
    p->type = partition_type_;

    uint32_t endLbn = relative_sector_ + partition_size_ - 1;
    c = lbn_to_cylinder(endLbn);
    if (c <= 1023) {
        p->endCylinderHigh = c >> 8;
        p->endCylinderLow = c & 0XFF;
        p->endHead = lbn_to_head(endLbn);
        p->endSector = lbn_to_sector(endLbn);
    }
    else {
        // too big flag, c = 1023, h = 254, s = 63
        p->endCylinderHigh = 3;
        p->endCylinderLow = 255;
        p->endHead = 254;
        p->endSector = 63;
    }
    p->firstSector = relative_sector_;
    p->totalSectors = partition_size_;

    if (!write_cache(0)) {
        logerror("write MBR");
        return false;
    }

    return true;
}

uint32_t FormatSdCard::volume_serial_number() {
    return (card_size_blocks_ << 8) + micros();
}

bool FormatSdCard::clear_fat_directory(uint32_t bgn, uint32_t n) {
    clear_cache(false);

    if (!card_.writeStart(bgn, n)) {
        logerror("clear_fat_directory writeStart failed");
        return false;
    }

    for (uint32_t i = 0; i < n; i++) {
        if (!card_.writeData(cache_.data)) {
            logerror("clear_fat_directory writeData failed");
            return false;
        }
    }

    if (!card_.writeStop()) {
        logerror("clear_fat_directory writeStop failed");
        return false;
    }

    return true;
}

}

#endif
