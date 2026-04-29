# 🚦 HVLI-KA: High-Priority Violation Logging and Intelligent Knowledge of Infractions 🏍️

## 📝 Description
**HVLI-KA** is a specialized, intelligent management system engineered to solve the critical issue of inconsistent enforcement and inefficiency during traffic checkpoint operations. By providing a structured, data-driven platform for traffic officers, it enables the objective recording, organization, and rapid retrieval of motorcycle violation records such as driving without a license or improper helmet use . This solution replaces ambiguous manual processes with a streamlined digital interface, bringing clarity, fairness, and accountability to road safety enforcement.

---

## 🎯 Objectives
* **Eliminate Profiling**: Minimize inspection disparities and concerns regarding the unfair targeting of motorcycle riders.
* **Standardize Enforcement**: Provide a robust, data-driven system to manage checkpoint violations objectively and transparently.
* **Optimize Operations**: Enable officers to quickly add violations, search records, generate reports, or undo entries through a streamlined menu.
* **Enhance Road Safety**: Prioritize high-risk or severe offenses to ensure that urgent safety threats are addressed immediately.

---

## 👥 Collaborators
| Profile | Name 👤 | Role 🛠️ | SR-Code 🆔 |
| :---: | :--- | :--- | :--- |
| <img src="./assets/ashanti.jpg" width="100" height="80"> | **Abdon, Alessandra Ashanti M.** | Leader, Documenter, Programmer | 25-00731 |
| <img src="./assets/tristan.jpg" width="100" height="80"> | **Bonto, Tristan James** | Member, Documenter, Programmer | 25-01894 |
| <img src="./assets/samantha.jpg" width="100" height="80"> | **Perez, Samantha Isabel S.** | Member, Documenter, Programmer | 25-05736 |

---

## 🛑 What Problem Are We Solving?
Traffic checkpoint operations frequently suffer from inconsistent enforcement practices where motorcycle riders are disproportionately flagged because their violations are highly visible. This leads to concerns regarding profiling, discrimination, and a lack of public trust. Furthermore, unclear prioritization criteria and ineffective manual recording can undermine road safety efforts. **HVLI-KA** resolves these issues by:
* **Ensuring Fairness**: Creating an objective, chronological queue for inspections.
* **Intelligent Sorting**: Automating the prioritization of severe safety risks (Major Violations).
* **Transparency**: Digitizing violation history to allow for instant, fair retrieval of data and reduction of administrative delays.

---

## 🏗️ Data Structures Used
| Data Structure | Purpose / Feature | Reason 💡 |
| :--- | :--- | :--- |
| **Stack** | Undo last violation entry | **LIFO** (Last-In, First-Out) is ideal for undoing the most recent action quickly. |
| **Queue** | Manage motorcycle queue | **FIFO** (First-In, First-Out) reflects a fair, first-come, first-served inspection order. |
| **Priority Queue** | Prioritize high-risk violations | Ensures urgent or severe offenses (Major) are handled first. |
| **Hash Table** | Store and retrieve records | Enables fast lookup of violation history by plate number. |
| **Array / List** | Store types of violations | Provides a clean way to organize, iterate, and display all offense categories. |

---

## 🚀 Key Features
* **✅ Categorized Violation Entry**: Provides distinct workflows for Major (e.g., DUI, Reckless Driving) and Minor infractions to ensure accurate reporting.
* **⚡ Intelligent Prioritization**: Uses a priority queue to automatically push high-risk violations to the front of the officer's task list.
* **🔍 Instant Search**: Offers fast lookup capabilities via a hash table to check a rider's history by plate number immediately.
* **⏪ Safety "Undo"**: Features a stack-based safeguard to reverse human errors during data entry.
* **📊 Reporting Tools**: Automatically compiles violation data into a report to support evidence-based enforcement decisions.

---

## 🏁  Closing Statement

Whether you're a "Major" rule-follower or a "Minor" rule-breaker, remember: **HVLI-KA** is watching! 👁️ Don't let your plate number end up in our Hash Table. Drive safely, wear your helmet properly, and let's keep the roads of Batangas (and the world) chaos-free!🏍️✨

*P.S. If you try to "Undo" your way out of a real ticket... nice try, but that only works in our code!* 🤪💻

---

