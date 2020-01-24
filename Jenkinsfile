@Library('conservify') _

conservifyProperties()

timestamps {
    node () {
        conservifyBuild(name: 'firmware', target: "ci", clean: "veryclean", archive: "build/*.zip, build/samd51/bootloader/*.elf, build/samd51/bootloader/*.bin, build/*/fk/*.bin, build/*/fk/*.elf")

		stage ('distribute') {
			withCredentials([usernamePassword(credentialsId: 'fkpassword', usernameVariable: 'FK_USER', passwordVariable: 'FK_PASSWORD')]) {
				distributeFirmware(profile: 'standard', module: 'fk-core', file: "build/samd51/bootloader/fkbl-fkb.bin", email: "${env.FK_USER}", password: "${env.FK_PASSWORD}")
				distributeFirmware(profile: 'standard', module: 'fk-bl', file: "build/samd51/fk/fk-bundled-fkb.bin", email: "${env.FK_USER}", password: "${env.FK_PASSWORD}")
            }
		}
    }

    refreshDistribution()
}
