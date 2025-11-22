# Medical-store-management-
A lightweight, console-based Medical Store Management system in C — simple, fast, and ideal for small pharmacies to manage customers, medicines, bills, stock and suppliers without a database.
Medical Store Management is a beginner-friendly, terminal-based application written in standard C. It implements core pharmacy workflows — customer registration, medicine inventory with expiry validation, billing with tax calculation and timestamping, supplier records and stock adjustments. All data persists to human-readable text files, making it easy to inspect or back up. The program focuses on practicality and reliability rather than GUI complexity, so it runs on minimal hardware and can be compiled and used in any standard C environment.
One-line app-store style blurb
A no-nonsense, console-based pharmacy manager in C — fast, portable, and ideal for small stores and learners.
Key features

Customer management: add, view, edit and remove customers (with phone, age, unique ID).

Medicine management: record name, MFG/EXP dates (validated), price and ID.

Billing: create bills from existing/new customers, compute tax, itemized bill storage and printing to console.

Stock control: add/update stock quantities by medicine ID, prevent negative stock.

Supplier records: manage suppliers and the list of medicines they provide.

Search utilities: search across customers, medicines, bills and suppliers by ID, name or phone number.

File-based persistence: all data is stored in readable .txt files (no external DB).

Input validation: date checks (including leap years), ID uniqueness checks, length checks for names/numbers.
