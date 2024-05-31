"use client"; // HeroSection.js
import React, { useState, useEffect } from "react";
import { motion } from "framer-motion";
import { TypeAnimation } from "react-type-animation";
import ButtonGroup from "./ButtonGroup";
import SocialIcons from "./SocialIcons";

const HeroSection = () => {
  const [isLoaded, setIsLoaded] = useState(false);
  const [isHovered, setIsHovered] = useState(false);

  useEffect(() => {
    setIsLoaded(true);
  }, []);

  return (
    <motion.section
      initial={{ opacity: 0 }}
      animate={{ opacity: 1 }}
      transition={{ duration: 1.5 }}
      className="relative h-screen"
      style={{
        backgroundImage: "url('/Images/esp32.jpg')",
        backgroundSize: "cover",
        backgroundPosition: "center",
      }}
    >
      <div>{/* Overlay */}</div>
      <div className="grid grid-cols-1 sm:grid-cols-12 relative h-full z-10">
        <div className="col-span-7 place-self-center text-center sm:text-left">
          <div className="text-right absolute top-0 right-0 mt-20 mr-10">
            <h1 className="text-white mb-4 text-xl lg:text-6xl font-extrabold">
              <span className="text-transparent bg-clip-text bg-gradient-to-r from-green-500 to-blue-500">
                Hello, I'm{" "}
              </span>
              <br></br>
              <div className="text-right">
                <TypeAnimation
                  sequence={[
                    "A SmartWatch",
                    1000,
                    "Not very smart",
                    1000,
                    "Taking your picture",
                    1000,
                    "Pedro Pedro Pedro",
                    1000,
                  ]}
                  wrapper="span"
                  speed={50}
                  repeat={Infinity}
                />
              </div>
            </h1>
            <div className="flex justify-end">
              <ButtonGroup setIsHovered={setIsHovered} />
            </div>
            <SocialIcons isHovered={isHovered} setIsHovered={setIsHovered} />
          </div>
        </div>
        <div className="col-span-5 hidden sm:block"></div>
      </div>
    </motion.section>
  );
};

export default HeroSection;
