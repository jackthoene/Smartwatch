"use client";
import React, { useTransition, useState } from "react";
import Image from "next/image";
import { motion } from "framer-motion"; // Import motion from framer-motion
import TabButton from "./TabButton";

const TAB_DATA = [
  {
    title: "Military",
    id: "military",
    content: (
      <ul className="list-disc pl-2">
        <li>Item 1</li>
        <li>Item 2</li>
        <li>Item 3</li>
        <li>Item 4</li>
      </ul>
    ),
  },
  {
    title: "Civilian",
    id: "civilian",
    content: (
      <ul className="list-disc pl-2">
        <li>Item 5</li>
        <li>Item 6</li>
        <li>Item 7</li>
        <li>Item 8</li>
      </ul>
    ),
  },
  {
    title: "Volunteer",
    id: "volunteer",
    content: (
      <ul className="list-disc pl-2">
        <li>Item 9</li>
        <li>Item 10</li>
        <li>Item 11</li>
        <li>Item 12</li>
      </ul>
    ),
  },
];

const WorkSection = () => {
  const [tab, setTab] = useState("military"); // Default tab set to "military"
  const [isPending, startTransition] = useTransition();

  const handleTabChange = (id) => {
    startTransition(() => {
      setTab(id);
    });
  };

  return (
    <motion.section
      initial={{ opacity: 0 }}
      animate={{ opacity: 1 }}
      transition={{ duration: 1 }}
      className="text-white"
    >
      <div className="md:grid md:grid-cols-2 gap-8 items-center py-8 px-4 xl:gap-16 sm:py-16 xl:px16">
        <Image
          src="/Images/work.jpg"
          alt="work image"
          width={1500}
          height={1500}
        />
        <div className="mt-4 md:mt-0 text-left flex flex-col h-full">
          <h2 className="text-4xl font-bold text-white mb-4">
            My work history
          </h2>
          <p className="text-base med:text-lg">
            • Versatile Engineer and Researcher experienced in a range of skills
            including Full Stack Engineering, Low Power Circuit Design, FPGA and
            ASIC design, Network Engineering, Cybersecurity, and Additive
            Manufacturing
          </p>
          <p className="text-base med:text-lg">
            • US Marine Corps Communications Officer: 7 Years of experience
            leading technical and innovative teams from the front
          </p>
          <p className="text-base med:text-lg">
            • Decisive leadership style that quickly moves from problem
            identification, analysis, bias for action and delivery of results.
          </p>
          <p className="text-base med:text-lg">
            • Dedicated to a sustainable future: researching new, innovative
            ways of computing and circularity in consumer-level products for a
            future that is more compatible with our surrounding environments
          </p>
          <p className="text-base med:text-lg">
            • Avid volunteer: 200 hours spent supporting Veterans non-profits
            and teaching engineering to low-opportunity demographics
          </p>
          <div className="flex flex-row mt-8">
            <TabButton
              selectTab={() => handleTabChange("military")}
              active={tab === "military"}
            >
              {" "}
              Military{" "}
            </TabButton>
            <TabButton
              selectTab={() => handleTabChange("civilian")}
              active={tab === "civilian"}
            >
              {" "}
              Civilian{" "}
            </TabButton>
            <TabButton
              selectTab={() => handleTabChange("volunteer")}
              active={tab === "volunteer"}
            >
              {"  "}
              Volunteer
              {"  "}
            </TabButton>
          </div>
          <div className="mt-8">
            {TAB_DATA.find((t) => t.id === tab).content}
          </div>
        </div>
      </div>
    </motion.section>
  );
};

export default WorkSection;
