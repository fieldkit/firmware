@Library('conservify') _

conservifyProperties()

def uploadFirmware(Map parameters = [:]) {
	def command = "--scheme https"

	if (parameters.directory) {
		command += " --firmware-directory " + parameters.directory
	}

	if (parameters.file) {
		command += " --firmware-file " + parameters.file
	}

	if (parameters.module) {
		command += " --module " + parameters.module
	}

	if (parameters.profile) {
		command += " --profile " + parameters.profile
	}

	if (parameters.email) {
		command += " --email " + parameters.email
	}

	if (parameters.password) {
		command += " --password " + parameters.password
	}

	if (parameters.version) {
		command += " --version " + parameters.version
	}

	def dev = "fktool  --host api.fkdev.org " + command
	echo dev
	sh dev

	def prod = "fktool  --host api.fieldkit.org " + command
	echo prod
	sh prod
}

timestamps {
    node () {
		try {
			stage ('build') {
				final scm = checkout scm

				def (remote, branch) = scm.GIT_BRANCH.tokenize('/')

				sh "env"

				sh "make veryclean"

				withEnv(["GIT_LOCAL_BRANCH=${branch}"]) {
					withEnv(["PATH+GOLANG=${tool 'golang-amd64'}/bin"]) {
						withEnv(["PATH+GOHOME=${HOME}/go/bin"]) {
							withPythonEnv("python") {
								sh "make ci"
							}
						}
					}
				}
			}

			stage ('test') {
				sh "make test"
			}

			stage ('archive') {
				archiveArtifacts artifacts: "build/*.zip, build/samd51/bootloader/*.elf, build/samd51/bootloader/*.bin, build/*/fk/*.bin, build/*/fk/*.elf, build/*/modules/*/*/*.bin, build/*/modules/*/*/*.elf"
			}

			stage ('distribute') {
				def version = readFile('build/samd51/version.txt')

				currentBuild.description = version

				withCredentials([usernamePassword(credentialsId: 'fkpassword', usernameVariable: 'FK_USER', passwordVariable: 'FK_PASSWORD')]) {
					uploadFirmware(version: version, profile: 'standard', module: 'fk-core', file: "build/samd51/fk/fk-bundled-fkb.bin", email: "${env.FK_USER}", password: "${env.FK_PASSWORD}")
					uploadFirmware(version: version, profile: 'standard', module: 'fk-bl', file: "build/samd51/bootloader/fkbl-fkb.bin", email: "${env.FK_USER}", password: "${env.FK_PASSWORD}")
				}
			}

			notifySuccess()
		}
		catch (Exception e) {
			notifyFailure()
			throw e;
		}
    }

    refreshDistribution()
}
