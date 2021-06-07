@Library('conservify') _

conservifyProperties()

def getBranch(scmInfo) {
	def (remoteOrBranch, branch) = scmInfo.GIT_BRANCH.tokenize('/')
	if (branch) {
		return branch;
	}
	return remoteOrBranch;
}

timestamps {
    try {
		node ("jenkins-aws-ubuntu") {
			def scmInfo

			stage ('git') {
				scmInfo = checkout scm
			}

			def branch = getBranch(scmInfo)

			stage ('clean') {
				sh "make clean"
			}

			withEnv(["GIT_LOCAL_BRANCH=${branch}"]) {
				stage ('build') {
					sh "make"
				}
			}

			def version = readFile('build/version.txt')

			currentBuild.description = version.trim()

			stage ('build') {
				sh "make test"
			}
		}
        notifySuccess()
    }
    catch (Exception e) {
        notifyFailure()
        throw e;
    }
}
