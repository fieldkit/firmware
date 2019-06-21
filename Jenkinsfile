@Library('conservify') _

conservifyProperties()

timestamps {
    node () {
        conservifyBuild(name: 'firmware', archive: "build/samd51/bootloade/*.elf, build/samd51/bootloade/*.bin, build/samd51/fk/*.bin, build/samd51/fk/*.fkb, build/samd51/fk/*.elf")
    }

    refreshDistribution()
}
