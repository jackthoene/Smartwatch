import Navbar from "../components/Navbar";
import EducationSection from "../components/EducationSection";

export default function projects() {
  return (
    <main className="flex min-h-screen flex-col bg-[#121212]">
      <Navbar />
      <div className="container mt-24 mx-auto px-12 py-4">
        <EducationSection />
      </div>
    </main>
  );
}
