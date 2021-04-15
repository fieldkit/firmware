#include <directory_chain.h>
#include <file_appender.h>
#include <working_buffers.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

class AllocFixture : public PhylumFixture {
protected:
    FlashMemory memory{ 256 };

};

TEST_F(AllocFixture, Allocate) {

}
