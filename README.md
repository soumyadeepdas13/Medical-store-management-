🏥 Medical Store Management System (C Console App)
___

📖 Overview

A lightweight console-based Medical Store Management System built in C.
It helps manage:
	•	Customers
	•	Medicines
	•	Stock levels
	•	Suppliers
	•	Billing & sales history

All data is stored in simple .txt files → no database required.

___

💡 Why This Project?
	•	Excellent learning project for file I/O, structs, and menus in C
	•	Fully offline — works on any OS with a C compiler
	•	Realistic simulation of a pharmacy management workflow
	•	Beginner-friendly but covers important programming concepts
___

✨ Features

👥 Customer Management
	•	Add, view, edit, delete customers
	•	Search by ID or phone number

💊 Medicine Management
	•	Add medicines with MFG & EXP dates
	•	Validate date inputs
	•	Prevent duplicate IDs

📦 Stock Management
	•	Track stock quantities
	•	Auto-deduct stock after billing
	•	Prevent negative stock

🏭 Supplier Management
	•	Add & manage supplier records

🧾 Billing System
	•	Generate bills
	•	Add multiple items
	•	Calculate totals & tax
	•	Save bill history

🔍 Search
	•	Search medicines, customers, suppliers, and past bills

___

🛠 Tech Stack
	•	Language: C
	•	Storage: .txt files
	•	Compiler: GCC / Clang / MinG

___

🚀 How to Run

Compile
gcc version5.c -o medical_store

Run 
medical_store.exe

___

🎯 Example Output 

==========================
 Medical Store Management
==========================

1. Customer Management
2. Medicine Management
3. Billing
4. Stock Management
5. Supplier Management
6. Search
0. Exit

Enter your choice:
___


🔮 Future Improvements
	•	Add login / password authentication
	•	Add low-stock alerts
	•	Export sales reports
	•	Add GUI version
	•	Add database support (MySQL / SQLite)
	•	Better error handling and validation
___


