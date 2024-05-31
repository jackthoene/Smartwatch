import Navbar from "../components/Navbar";
import ResearchSection from "../components/ResearchSection";

export default function About() {
  return (
    <main className="flex min-h-screen flex-col bg-[#121212]">
      <Navbar />
      <div className="container mt-24 mx-auto px-12 py-4">
        <ResearchSection />
      </div>
    </main>
  );
}
