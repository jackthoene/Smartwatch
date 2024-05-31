"use client";
import React, { useState, useRef } from "react";
import ResearchCard from "./ResearchCard";
import ResearchTag from "./ResearchTag";
import { motion, useInView } from "framer-motion";

const researchData = [
  {
    id: 1,
    title: "React Portfolio Website",
    description: "research 1 description",
    image: "/Images/coming-soon.png",
    tag: ["All", "Web"],
    gitUrl: "/",
    previewUrl: "/",
  },
  {
    id: 2,
    title: "Potography Portfolio Website",
    description: "research 2 description",
    image: "/Images/coming-soon.png",
    tag: ["All", "Web"],
    gitUrl: "/",
    previewUrl: "/",
  },
  {
    id: 3,
    title: "E-commerce Application",
    description: "research 3 description",
    image: "/Images/coming-soon.png",
    tag: ["All", "Web"],
    gitUrl: "/",
    previewUrl: "/",
  },
  {
    id: 4,
    title: "Food Ordering Application",
    description: "research 4 description",
    image: "/Images/coming-soon.png",
    tag: ["All", "Mobile"],
    gitUrl: "/",
    previewUrl: "/",
  },
  {
    id: 5,
    title: "React Firebase Template",
    description: "Authentication and CRUD operations",
    image: "/Images/coming-soon.png",
    tag: ["All", "Web"],
    gitUrl: "/",
    previewUrl: "/",
  },
  {
    id: 6,
    title: "Full-stack Roadmap",
    description: "research 5 description",
    image: "/Images/coming-soon.png",
    tag: ["All", "Web"],
    gitUrl: "/",
    previewUrl: "/",
  },
];

const ResearchSection = () => {
  const [tag, setTag] = useState("All");
  const ref = useRef(null);
  const isInView = useInView(ref, { once: true });

  const handleTagChange = (newTag) => {
    setTag(newTag);
  };

  const filteredresearch = researchData.filter((research) =>
    research.tag.includes(tag)
  );

  const cardVariants = {
    initial: { y: 50, opacity: 0 },
    animate: { y: 0, opacity: 1 },
  };

  return (
    <section id="research">
      <h2 className="text-center text-4xl font-bold text-white mt-4 mb-8 md:mb-12">
        My research
      </h2>
      <div className="text-white flex flex-row justify-center items-center gap-2 py-6">
        <ResearchTag
          onClick={handleTagChange}
          name="All"
          isSelected={tag === "All"}
        />
        <ResearchTag
          onClick={handleTagChange}
          name="Web"
          isSelected={tag === "Web"}
        />
        <ResearchTag
          onClick={handleTagChange}
          name="Mobile"
          isSelected={tag === "Mobile"}
        />
      </div>
      <ul ref={ref} className="grid md:grid-cols-3 gap-8 md:gap-12">
        {filteredresearch.map((research, index) => (
          <motion.li
            key={index}
            variants={cardVariants}
            initial="initial"
            animate={isInView ? "animate" : "initial"}
            transition={{ duration: 0.3, delay: index * 0.15 }}
          >
            <ResearchCard
              key={research.id}
              title={research.title}
              description={research.description}
              imgUrl={research.image}
              gitUrl={research.gitUrl}
              previewUrl={research.previewUrl}
            />
          </motion.li>
        ))}
      </ul>
    </section>
  );
};

export default ResearchSection;
