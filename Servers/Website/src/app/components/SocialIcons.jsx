// SocialIcons.js
import React from "react";

const SocialIcons = ({ isHovered, setIsHovered }) => {
  return (
    <div
      className={
        isHovered
          ? "bg-gradient-to-r from-green-500 to-blue-500 rounded-full bg-opacity-5 p-2  flex justify-center items-center absolute top-full left-1/2 transform -translate-x-1/2"
          : "hidden"
      }
      onMouseEnter={() => setIsHovered(true)}
      onMouseLeave={() => setIsHovered(false)}
    >
      <a
        href="https://www.linkedin.com/in/jack-thoene/"
        target="_blank"
        rel="noopener noreferrer"
        className="mx-2"
      >
        <img src="/Images/linkedin.png" alt="LinkedIn" className="w-8 h-8" />
      </a>
      <a
        href="https://github.com/jackthoene"
        target="_blank"
        rel="noopener noreferrer"
        className="mx-2"
      >
        <img src="/Images/github.png" alt="GitHub" className="w-8 h-8" />
      </a>
      <a
        href="https://vaklab.wordpress.com/"
        target="_blank"
        rel="noopener noreferrer"
        className="mx-2"
      >
        <img src="/Images/vak.png" alt="VAK Lab" className="w-8 h-8" />
      </a>
      <a
        href="https://www.sci-mi.org/index.html"
        target="_blank"
        rel="noopener noreferrer"
        className="mx-2"
      >
        <img src="/Images/scimi.png" alt="SCI-MI" className="w-8 h-8" />
      </a>
      <a
        href="https://www.linkedin.com/company/marine-coders/about/"
        target="_blank"
        rel="noopener noreferrer"
        className="mx-2"
      >
        <img
          src="/Images/marinecoders.png"
          alt="Marine Coders"
          className="w-8 h-8"
        />
      </a>
      <a
        href="https://www.pbabbate.org/home-0"
        target="_blank"
        rel="noopener noreferrer"
        className="mx-2"
      >
        <img
          src="/Images/pba.png"
          alt="Patrol Base Abbate"
          className="w-8 h-8"
        />
      </a>
      {/* Add more social media icons as needed */}
    </div>
  );
};

export default SocialIcons;
