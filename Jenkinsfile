@Library('conservify') _

conservifyProperties()

timestamps {
    try {
		node ("jenkins-aws-ubuntu") {
			stage ('git') {
				checkout scm
			}

			stage ('clean') {
				sh "make clean"
			}

			stage ('build') {
				sh "make"
			}

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
