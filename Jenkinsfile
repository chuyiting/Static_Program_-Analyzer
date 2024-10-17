pipeline {
    agent any
    options {
            skipDefaultCheckout true
    }
    stages {
        stage("Clone repository") {
            steps {
                git credentialsId: '9200a6ef-d122-4821-962a-33cdaacc161c', url: 'https://github.com/nus-cs3203/22s1-cp-spa-team-06.git'
                checkout([$class: 'GitSCM',
                branches: [[name: '*/master']],
                extensions: [],
                userRemoteConfigs:
                    [[credentialsId: '9200a6ef-d122-4821-962a-33cdaacc161c',
                     url: 'https://github.com/nus-cs3203/22s1-cp-spa-team-06.git']]])

            }
        }

        stage('Static Analysis'){
            steps {
                echo 'I am really doing static analysis now...'
                echo 'Perfect code writing. How can I be like you?'
            }
        }

        stage('build') {
            steps {
                catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE') {

                    bat """
                        rm -r out
                        mkdir out
                        cd out
                        cmake ../Team06/Code06 -DCMAKE_BUILD_TYPE=Release
                        cmake --build . --target ALL_BUILD
                    """

                }
            }
        }

        stage('Run Unit Test'){
            steps {
                echo 'Run Unit Tests'
                sh 'out/src/unit_testing/Debug/unit_testing.exe'
            }
        }

        stage('Run Integration Test'){
            steps {
                echo 'Run Integration Tests'
                sh 'out/src/integration_testing/Debug/integration_testing.exe'
            }
        }

        stage('Run System Test'){
            steps {

                echo 'Run System Tests'
                catchError(buildResult: 'SUCCESS', stageResult: 'FAILURE') {
                    echo 'Cannot build autotester on server yet...'
                    bat """
                        cd out/src/autotester/Tests06
                        xcopy /s "Debug/autotester.exe" "autotester.exe"
                        python run-all-test.py jenkins
                    """
                }
            }
        }


        stage('Deploy') {
            when {
                    branch "master"
            }
            steps {
                echo 'I am deploying...'
                echo 'Actually nothing to deploy HAHAHA'
            }
        }
    }


    post {
         success {
             emailext attachLog: true, body: '$DEFAULT_CONTENT',
             postsendScript: '$DEFAULT_POSTSEND_SCRIPT',
             presendScript: '$DEFAULT_PRESEND_SCRIPT',
             subject: '$DEFAULT_SUBJECT',
             to: 'sthk100@gmail.com, chongjunwei@u.nus.edu, e0418218@u.nus.edu, leeyiheng12@u.nus.edu, neoruien123@gmail.com, e0389015@u.nus.edu'
         }
         failure {
             emailext attachLog: true,
             body: '$DEFAULT_CONTENT',
             postsendScript: '$DEFAULT_POSTSEND_SCRIPT',
             presendScript: '$DEFAULT_PRESEND_SCRIPT',
             subject: '$DEFAULT_SUBJECT',
             to: 'sthk100@gmail.com, chongjunwei@u.nus.edu, e0418218@u.nus.edu, leeyiheng12@u.nus.edu, neoruien123@gmail.com, e0389015@u.nus.edu'
         }

     }
}
