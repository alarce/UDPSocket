
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
                dir('server'){
                    sh 'g++ main.cpp -o ../server_bin'
                }
                dir('client'){
                    sh 'g++ main.cpp -o ../client_bin'
                }
                
                echo 'Compilación completada.'
            }
        }
        stage('Pruebas (Test)') {
            steps {
                echo 'Ejecutando pruebas unitarias...'
                sh 'g++ test/tests.cpp -o test/ejecutable_tests && ./test/ejecutable_tests'
                echo '¡Pruebas superadas!'
            }
        }
        stage('Despliegue (Deploy)') {
            steps {
                echo 'Subiendo archivos al servidor de producción...'
                // Esto hace que el archivo aparezca para descargar en la web
                archiveArtifacts artifacts: 'mi_programa', fingerprint: true
                echo 'Despliegue finalizado con éxito.'
            }
        }
    }
}