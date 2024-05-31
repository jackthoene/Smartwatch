// App.js
import React from "react";
import HeroSection from "./components/HeroSection";
import Navbar from "./components/Navbar";
import LiveStream from "./components/LiveStream"; // Adjust the import path as necessary

function App() {
  return (
    <main className="flex min-h-screen flex-col bg-[#121212]">
      <Navbar />
      <div className="container mt-24 mx-auto px-12 py-4">
        <HeroSection />
        <LiveStream />
      </div>
    </main>
  );
}

export default App;
