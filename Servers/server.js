const WebSocket = require('ws');
const fs = require('fs');
const { exec } = require('child_process');

const server = new WebSocket.Server({ port: 8080 });

server.on('connection', socket => {
    console.log('A new client connected!');
    
    socket.on('message', message => {
        console.log('Message type:', typeof message);
        
        if (typeof message === 'object') {
            try {
                const data = JSON.parse(message);
                console.log('Data Type:', data.type);
                
                if (data.type === 'IMU') {
                    console.log('Received IMU data:', data);
                    fs.writeFileSync('imu_data.json', JSON.stringify(data, null, 2));
                } else {
                    console.error('Unknown JSON message type');
                }
            } catch (error) {
                console.error(`Error parsing message as JSON: ${error}`);
            }
        } else {
            // Assuming that binary messages are image data
            console.log('Received binary image data');
            
            // Save the received data to a file
            fs.writeFile('image.jpg', message, err => {
                if (err) {
                    console.error('Error saving image:', err);
                } else {
                    console.log('Image saved as image.jpg');
                    exec('python Face_recognition.py', (error, stdout, stderr) => {
                        if (error) {
                            console.error(`Error executing Face_recognition.py: ${error}`);
                            return;
                        }
                        console.log(`Face_recognition.py output: ${stdout}`);
                    });
                }
            });
        }
    });

    socket.on('close', () => {
        console.log('Client has disconnected.');
    });
});

console.log('WebSocket server is running on ws://localhost:8080');
