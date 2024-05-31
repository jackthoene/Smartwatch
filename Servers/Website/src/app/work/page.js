import Navbar from "../components/Navbar";
import WorkSection from "../components/WorkSection"

export default function work() {
  return (
    <main className="flex min-h-screen flex-col bg-[#121212]">
      <Navbar />
      <div className="container mt-24 mx-auto px-12 py-4">
        <WorkSection />
      </div>
    </main>
  );
}
