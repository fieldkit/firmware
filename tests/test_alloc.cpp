#include <directory_chain.h>
#include <file_appender.h>
#include <working_buffers.h>

#include "suite_base.h"
#include "geometry.h"

using namespace phylum;

class AllocSuite : public PhylumSuite {
protected:
    FlashMemory memory{ 256 };

};

TEST_F(AllocSuite, Allocate) {

}
