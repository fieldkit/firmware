@Library('conservify') _

conservifyProperties()

timestamps {
    node () {
        conservifyBuild(name: 'firmware', target: "ci", clean: "veryclean", archive: "build/*.zip, build/samd51/bootloader/*.elf, build/samd51/bootloader/*.bin, build/*/fk/*.bin, build/*/fk/*.elf")
    }

    refreshDistribution()
}
