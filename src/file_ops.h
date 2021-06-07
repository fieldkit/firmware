#include "super_chain.h"
#include "directory_tree.h"
#include "tree_sector.h"

namespace phylum {

template<typename directory_type, typename tree_type>
class file_ops {
private:
    phyctx pc_;
    super_chain &sc_;
    directory_type dir_;

public:
    file_ops(phyctx pc, super_chain &sc) : pc_(pc), sc_(sc), dir_{ pc, sc.directory_tree() } {
    }

public:
    phyctx &pc() {
        return pc_;
    }

    directory_type &dir() {
        return dir_;
    }

public:
    int32_t mount() {
        return 0;
    }

    int32_t format() {
        auto err = dir_.format();
        if (err < 0) {
            return err;
        }

        err = sc_.update(dir_.to_tree_ptr());
        if (err < 0) {
            return err;
        }

        return 0;
    }

    int32_t touch(const char *name, open_file_config file_cfg = {}) {
        auto err = dir_.template touch_indexed<tree_type>(name, file_cfg);
        if (err < 0) {
            return err;
        }

        err = sc_.update(dir_.to_tree_ptr());
        if (err < 0) {
            return err;
        }

        return 0;
    }

    int32_t index_if_necessary(file_appender &appender, record_number_t record_number) {
        return appender.index_if_necessary<tree_type>(record_number);
    }

    int32_t seek_position(file_reader &reader, file_size_t position) {
        auto err = reader.seek_position<tree_type>(position);
        if (err < 0) {
            return err;
        }

        return err;
    }

    int32_t seek_record(file_reader &reader, record_number_t record) {
        auto err = reader.seek_record<tree_type>(record);
        if (err < 0) {
            return err;
        }

        return err;
    }

};

}
