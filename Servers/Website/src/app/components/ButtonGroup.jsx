// ButtonGroup.js
import React from "react";

const ButtonGroup = ({ setIsHovered }) => {
  const handleNavigateToPhotos = () => {
    // Set the IP address or URL where the photos can be viewed
    const photosUrl = "http://192.168.107.77"; // Change this to your actual IP address

    // Use window.location to navigate to the specified URL
    window.location.href = photosUrl;
  };

  const handleMouseOver = () => {
    setIsHovered(true);
  };

  const handleMouseOut = () => {
    setIsHovered(false);
  };

  return (
    <div className="flex justify-end">
      <button
        className="px-6 py-3 rounded-full mr-4 bg-gradient-to-br from-green-500 to-blue-500 hover:bg-slate-200 text-black"
        onMouseEnter={handleMouseOver}
        onMouseLeave={handleMouseOut}
      >
        My Links
      </button>
      <button
        className="px-6 py-3 rounded-full bg-gradient-to-br from-green-500 to-blue-500 hover:bg-slate-800 text-white"
        onClick={handleNavigateToPhotos}
      >
        See Photos Taken
      </button>
    </div>
  );
};

export default ButtonGroup;
