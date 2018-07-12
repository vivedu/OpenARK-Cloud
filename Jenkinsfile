/**
 * @see https://github.com/vivedu/OpenARK-Cloud/issues/5
 */

// @see https://github.com/vivedu/VIVEDU-Store/issues/329
def abortPreviousBuilds() {
  def currentJobName = env.JOB_NAME
  def currentBuildNumber = env.BUILD_NUMBER.toInteger()
  def jobs = Jenkins.instance.getItemByFullName(currentJobName)
  def builds = jobs.getBuilds()

  for (build in builds) {
    if (!build.isBuilding()) {
      continue;
    }

    if (currentBuildNumber == build.getNumber().toInteger()) {
      continue;
    }

    build.doStop()
  }
}

pipeline {
  agent {
    docker { image 'ubuntu_image:latest' }
  }

  parameters {
    // 获取 jenkinsProjectName
    string(name: 'jenkinsProjectName', defaultValue: "${env.JOB_NAME.split('/')[0]}")
  }

  stages {
    stage('Install') {
      steps {
        echo "Project name: ${params.jenkinsProjectName}\nJob name: `${env.JOB_NAME}`\nBranch name: ${env.BRANCH_NAME}\nChange Title: ${env.CHANGE_TITLE}\nBuild ID: ${env.BUILD_ID}\nJenkinsURL: ${env.JENKINS_URL}"

        // @see https://github.com/vivedu/VIVEDU-Store/issues/329
        abortPreviousBuilds()

        // 接入 slack 通知
        // sendSlackNotification("CI is triggered by `${env.JOB_NAME}`\n${env.JENKINS_URL}blue/organizations/jenkins/${params.jenkinsProjectName}/detail/${env.BRANCH_NAME}/${env.BUILD_ID}/pipeline/")
      }
    }

    stage('Build') {
      steps {
        sh "bash scripts/build.sh"
      }
    }
  }

  post {
    aborted {
      echo 'Build aborted'

      // 接入 slack 通知
      // sendSlackNotification("`${env.JOB_NAME}` is aborted.\n${env.JENKINS_URL}blue/organizations/jenkins/${params.jenkinsProjectName}/detail/${env.BRANCH_NAME}/${env.BUILD_ID}/pipeline/", "warning")
    }

    success {
       // archiveArtifacts artifacts: 'dist/**', fingerprint: true
    }

    failure {
       echo 'Build failed'

       // 接入 slack 通知
       // sendSlackNotification("Failed to build `${env.JOB_NAME}`.\n${env.JENKINS_URL}blue/organizations/jenkins/${params.jenkinsProjectName}/detail/${env.BRANCH_NAME}/${env.BUILD_ID}/pipeline/", "danger")
    }
  }
}
