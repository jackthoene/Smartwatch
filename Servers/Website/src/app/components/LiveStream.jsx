"use client";
// LiveStream.jsx
import React, { useState, useEffect } from "react";

const LiveStream = () => {
  const [imageSrc, setImageSrc] = useState("");

  useEffect(() => {
    const fetchAnimationFrame = () => {
      fetch("http://192.168.57.28:5000/animation")
        .then((response) => response.blob())
        .then((blob) => {
          setImageSrc(URL.createObjectURL(blob));
        })
        .catch((error) =>
          console.error("Error fetching animation frame:", error)
        );
    };

    const intervalId = setInterval(fetchAnimationFrame, 1000);

    return () => clearInterval(intervalId);
  }, []);

  return (
    <div className="image-container">
      {imageSrc ? (
        <img src={imageSrc} alt="Animated Pyramid" className="animated-image" />
      ) : (
        <p>Loading animation...</p>
      )}
    </div>
  );
};

export default LiveStream;
