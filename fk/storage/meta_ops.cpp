#include "storage/meta_ops.h"
#include "utilities.h"
#include "records.h"
#include "state.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("meta");

MetaOps::MetaOps(Storage &storage) : storage_(storage) {
}

tl::expected<uint32_t, Error> MetaOps::write_state(GlobalState *gs, Pool &pool) {
    return write_state(gs, &fkb_header, pool);
}

tl::expected<uint32_t, Error> MetaOps::write_state(GlobalState *gs, fkb_header_t const *fkb, Pool &pool) {
    auto meta = storage_.file(Storage::Meta);
    if (!meta.seek_end()) {
        FK_ASSERT(meta.create());
    }

    MetaRecord record;
    record.include_state(gs, fkb, pool);

    auto srl = SignedRecordLog{ meta };
    auto meta_record = srl.append_immutable(SignedRecordKind::State, &record.record(), fk_data_DataRecord_fields, pool);
    if (!meta_record) {
        return tl::unexpected<Error>(meta_record.error());
    }

    gs->update_meta_stream(meta);

    return (*meta_record).record;
}

tl::expected<uint32_t, Error> MetaOps::write_modules(GlobalState *gs, fkb_header_t const *fkb, ConstructedModulesCollection &modules, ModuleReadingsCollection &readings, Pool &pool) {
    auto meta = storage_.file(Storage::Meta);
    if (!meta.seek_end()) {
        FK_ASSERT(meta.create());
    }

    MetaRecord record;
    record.include_modules(gs, fkb, modules, readings, pool);

    auto srl = SignedRecordLog { meta };
    auto meta_record = srl.append_immutable(SignedRecordKind::Modules, &record.record(), fk_data_DataRecord_fields, pool);

    gs->update_meta_stream(meta);

    return (*meta_record).record;
}

}
