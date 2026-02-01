
pipeline {
    agent {
        docker {
            image 'gcc:latest'
        }
    }

    stages {
        stage('Construcción (Build)') {
            steps {
                echo 'Bajando el código del repositorio...'
                dir('server/src'){
                    sh 'g++ main.cpp util.cpp -I ../include -o ../server_bin'
                }
                dir('client/src'){
                    sh 'g++ main.cpp util.cpp -I ../include -o ../client_bin'
                }
                
                echo 'Compilación completada.'
            }
        }
        stage('Pruebas (Test)') {
            steps {
                echo 'Ejecutando pruebas unitarias...'
                dir('server/test'){
                    sh 'g++ tests.cpp ../src/util.cpp -I ../include -o ejecutable_tests && ./ejecutable_tests'
                }
                echo '¡Pruebas superadas!'
            }
        }
        stage('Despliegue (Deploy)') {
            steps {
                echo 'Subiendo archivos al servidor de producción...'
                // Esto hace que el archivo aparezca para descargar en la web
                archiveArtifacts artifacts: 'server/server_bin, client/client_bin', fingerprint: true
                echo 'Despliegue finalizado con éxito.'
            }
        }
    }
}