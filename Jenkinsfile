@Library('conservify') _

conservifyProperties()

timestamps {
    node () {
        conservifyBuild(name: 'firmware', archive: "build/samd51/bootloader/*.elf, build/samd51/bootloader/*.bin, build/samd51/fk/*.bin, build/samd51/fk/*.fkb, build/samd51/fk/*.elf")
    }

    refreshDistribution()
}
