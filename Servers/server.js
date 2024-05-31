const WebSocket = require('ws');
const fs = require('fs');
const { exec } = require('child_process');

const server = new WebSocket.Server({ port: 8080 });

server.on('connection', socket => {
    console.log('A new client connected!');

    let imageData = [];
    let imuData = {};

    socket.on('message', message => {
        if (message === 'IMAGE_START') {
            imageData = [];
            console.log('Started receiving image data');
        } else if (message === 'IMAGE_END') {
            console.log('Finished receiving image data');
            fs.writeFileSync('image.jpg', Buffer.concat(imageData));
            exec('python Face_recognition.py', (error, stdout, stderr) => {
                if (error) {
                    console.error(`Error executing Face_recognition.py: ${error}`);
                    return;
                }
                console.log(`Face_recognition.py output: ${stdout}`);
            });
        } else if (message === 'IMU_START') {
            imuData = { x: [], y: [], z: [] };
            console.log('Started receiving IMU data');
        } else if (message === 'IMU_END') {
            console.log('Finished receiving IMU data');
            fs.writeFileSync('imu_data.json', JSON.stringify(imuData, null, 2));
        } else {
            try {
                const data = JSON.parse(message);
                if (data.type === 'image') {
                    imageData.push(Buffer.from(data.data, 'base64'));
                } else if (data.type === 'imu') {
                    imuData.x.push(data.x);
                    imuData.y.push(data.y);
                    imuData.z.push(data.z);
                }
            } catch (error) {
                console.error(`Error parsing message: ${error}`);
            }
        }
    });

    socket.on('close', () => {
        console.log('Client has disconnected.');
    });
});

console.log('WebSocket server is running on ws://localhost:8080');
