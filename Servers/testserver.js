const WebSocket = require('ws');
const fs = require('fs');
const path = require('path');
const readline = require('readline');

// Create a WebSocket client that connects to the target server on port 8080
const targetServerUrl = 'ws://localhost:8080';
const client = new WebSocket(targetServerUrl);

client.on('open', () => {
  console.log('Connected to the target WebSocket server on port 8080');
});

client.on('error', (error) => {
  console.error('WebSocket error:', error);
});

client.on('close', () => {
  console.log('Connection to the target WebSocket server closed');
});

// Create a WebSocket server that listens on port 8081
const server = new WebSocket.Server({ port: 8081 });

server.on('connection', (ws) => {
  console.log('Client connected to the WebSocket server on port 8081');

  ws.on('message', (message) => {
    console.log('Received message from client:', message);
  });

  ws.on('close', () => {
    console.log('Client disconnected from the WebSocket server');
  });
});

console.log('WebSocket server is listening on port 8081');

// Set up readline to listen for user input from the terminal
const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

rl.on('line', (input) => {
  if (input === 'send image') {
    // Read the image file
    const imagePath = path.join(__dirname, 'image2.jpg');
    fs.readFile(imagePath, (err, data) => {
      if (err) {
        console.error('Error reading the image file:', err);
        return;
      }

      // Send the image data to the target server as binary
      client.send(data, { binary: true }, (error) => {
        if (error) {
          console.error('Error sending the image:', error);
        } else {
          console.log('Image sent to the target server');
        }
      });
    });
  } else if (input === 'send imu') {
    // Generate random IMU data
    const imuData = {
      type: 'IMU',
      x: (Math.random() * 10).toFixed(2),
      y: (Math.random() * 10).toFixed(2),
      z: (Math.random() * 10).toFixed(2)
    };

    // Send the IMU data in JSON format
    client.send(JSON.stringify(imuData));
    console.log('IMU data sent to the target server:', imuData);
  } else {
    console.log('Unknown command');
  }
});
