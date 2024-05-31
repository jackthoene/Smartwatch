import Navbar from "../components/Navbar";
import ContactSection from "../components/ContactSection";

export default function Contacts() {
  return (
    <main className="flex min-h-screen flex-col bg-[#121212]">
      <Navbar />
      <div className="container mt-24 mx-auto px-12 py-4">
        <ContactSection />
      </div>
    </main>
  );
}
