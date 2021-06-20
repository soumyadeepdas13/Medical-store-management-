#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>

char wasteCh;
char customerFileName[] = "CustomerDetails.txt";
char medicineFileName[] = "MedicineDetails.txt";
char billFileName[] = "BillDetails.txt";
char stockFileName[] = "StockDetails.txt";
char supplierFileName[] = "SupplierDetails.txt";

int noOfCustomers = 0;
int noOfMedicines = 0;
int noOfBills = 0;
int noOfStocks = 0;
int noOfSuppliers = 0;
int start=0;

struct Date
{
    int dd, mm, yyyy;
};
struct CustomerDetails
{
    char name[20], number[12];
    int age, ID;
};
struct MedicineDetails
{
    char name[20];
    struct Date mfgDate;
    struct Date expDate;
    int amount, ID;
};
struct BillDetails
{
    struct CustomerDetails customer;
    int quantityOfMedicine;
    struct MedicineDetails medicine[10];
    int qtyMed[10];
    struct Date date;
    float totalAmount, tax;
    int ID;
};
struct StockDetails
{
    int medicineID;
    int quantity;
};
struct SupplierDetails{
    char name[20],number[12],medName[10][20];
    int medicineID[10],ID,noOfMedProvide;
};

struct CustomerDetails customerArr[50];
struct MedicineDetails medicineArr[50];
struct BillDetails billArr[50];
struct StockDetails stockArr[50];
struct SupplierDetails supplierArr[50];

int isLeap(int year);
void inputDate(struct Date *d);
void inputCustomer(struct CustomerDetails *cd);
void inputMedicine(struct MedicineDetails *md);
void inputBill(struct BillDetails *bd);
void inputStock(struct StockDetails *sd,int ID);
void inputSupplier(struct SupplierDetails *supd);

void readCustomerFile();
void writeCustomerFile(int rm);
void viewCustomerDetails();
void removeCustomerDetails();
void editCustomerDetails();

void readMedicineDetails();
void writeMedicineDetails(int rm);
void viewMedicineDetails();
void removeMedicineDetails();

void readBills();
void writeBills(int rm);
void billOfIndex(int i);
void viewBills(int ID);
void removeBills();

void readStock();
void writeStock(int rm);
void viewStock();
void removeStock();

void readSupplier();
void writeSupplier(int rm);
void supplierOfIndex(int i);
void viewSupplier(int ID);
void removeSupplier();
void editSupplier();

void nameInput(char* name,char str[]);
void numberInput(char* number,int len);
int inputNumber(int len,char str[]);
int inputChoice(int lastCh);

int checkMedicine(int ID);
int checkCustomer(int ID);
int checkBill(int ID);
int checkStock(int ID);
int checkSupplier(int ID);

void CustomerMain();
void MedicineMain();
void BillsMain();
void StockMain();
void SupplierMain();
void searchMain();

void searchID();
void searchName();
void searchNumber();

int main();

int isLeap(int year)
{
    if (year % 400 == 0)
        return 1;
    else if (year % 100 == 0)
        return 0;
    else if (year % 4 == 0)
        return 1;
    else
        return 0;
}
void inputDate(struct Date *d)
{
    char date[11], dc[3], mc[3], yc[5];
    int di, mi, yi;
    again1:
    fflush(stdin);
    scanf("%s", date);
    dc[0] = date[0], dc[1] = date[1], dc[2] = '\0';
    mc[0] = date[3], mc[1] = date[4], mc[2] = '\0';
    yc[0] = date[6], yc[1] = date[7], yc[2] = date[8], yc[3] = date[9], yc[4] = '\0';
    di = atoi(dc), mi = atoi(mc), yi = atoi(yc);
    if(!((mi > 0 && mi <= 12 && di > 0) &&
       (((mi == 1 || mi == 3 || mi == 5 || mi == 7 || mi == 8 || mi == 10 || mi == 12) && di <= 31) ||
        ((mi == 4 || mi == 6 || mi == 9 || mi == 11) && di <= 30) || (mi == 2 && di <= 28 && isLeap(yi) == 0) || (mi == 2 && di <= 29 && isLeap(yi) == 1))))
    {
        printf("\nInvalid date\nExample (07.09.2002)\nPlease use format (dd.mm.yyyy): ");
        goto again1;
    }
    if(!(yi >= 1980 && yi <= 2100)){
        printf("\nDate Out of bound!\nPlease enter in range (1980-2100): ");
        goto again1;
    }
    d->dd=di,d->mm=mi,d->yyyy=yi;
}
void inputCustomer(struct CustomerDetails *cd){
    printf("Enter the customer name: ");
    nameInput(cd->name,"customer");
    printf("Enter Ph.No.: ");
    numberInput(cd->number,10);
    printf("Enter age: ");
    cd->age=inputNumber(2,"age");
    if(cd->age<18){
        printf("\nAge below 18 is not valid for medical purchase!\nTry again with valid age!\n");
        sleep(2);
        CustomerMain();
    }
    printf("Enter ID (max 6 digits): ");
    again5:
    cd->ID=inputNumber(6,"ID");
    if (checkCustomer(cd->ID) >= 0)
    {
        printf("\nCustomer of same ID alredy present!\nPlease enter a different ID: ");
        goto again5;
    }
}
void inputMedicine(struct MedicineDetails *md){
    printf("\nEnter Medicine name: ");
    nameInput(md->name,"medicine");
    printf("Enter Mfg.date (dd.mm.yyyy): ");
    inputDate(&md->mfgDate);
    again6:
    printf("Enter Exp.date (dd.mm.yyyy): ");
    inputDate(&md->expDate);
    if (!((md->expDate.yyyy > md->mfgDate.yyyy)||
          (md->expDate.yyyy == md->mfgDate.yyyy && md->expDate.mm > md->mfgDate.mm)||
          (md->expDate.yyyy == md->mfgDate.yyyy && md->expDate.mm == md->mfgDate.mm && md->expDate.dd > md->mfgDate.dd)))
    {
        printf("Mfg.dt: %d.%d.%d\t",md->mfgDate.dd,md->mfgDate.mm,md->mfgDate.yyyy);
        printf("Exp.dt: %d.%d.%d\n",md->expDate.dd,md->expDate.mm,md->expDate.yyyy);
        printf("\nExp.dt cant be less than or same as Mfg.dt!\n");
        goto again6;
    }
    printf("Enter amount: ");
    md->amount=inputNumber(9,"amount");
    printf("Enter MedicineID: ");
    md->ID=inputNumber(6,"ID");
}
void inputBill(struct BillDetails *bd){
    int ch,ID,flagIndex;
    float totalAmt=0.0f;
    again8:
    if(noOfCustomers>0){
        printf("Please choose from the options below\n");
        printf("\t1. View existing customer before adding to the bill");
        printf("\n\t2. Existing customer\n\t3. New customer\nEnter option number: ");
        ch=inputChoice(3);
    } else {
        printf("No Customer in the system!\nPlease add new customer\n\n");
        ch=3;
    }
    if(ch==1){
        viewCustomerDetails();
        printf("\n");
        goto again8;
    } else if(ch==2){
        printf("Enter the customer ID (max 6 digits): ");
        ID=inputNumber(6,"ID");
        flagIndex=checkCustomer(ID);
        if(flagIndex>=0){
            strcpy(bd->customer.name, customerArr[flagIndex].name);
            strcpy(bd->customer.number, customerArr[flagIndex].number);
            bd->customer.age = customerArr[flagIndex].age;
            bd->customer.ID = customerArr[flagIndex].ID;
        } else {
            printf("Wrong Customer ID, Plese choose from the list\n");
            printf("%-20s %-12s%s  %s\n", "Name", "Number", "Age", "ID");
            for(int i = 0; i < noOfCustomers; i++)
                printf("%-20s%-12s %d   %d\n", customerArr[i].name, "********", customerArr[i].age, customerArr[i].ID);
            printf("\n");
            goto again8;
        }
    } else if(ch==3) {
        inputCustomer(&customerArr[noOfCustomers]);
        strcpy(bd->customer.name, customerArr[noOfCustomers].name);
        strcpy(bd->customer.number, customerArr[noOfCustomers].number);
        bd->customer.age = customerArr[noOfCustomers].age;
        bd->customer.ID = customerArr[noOfCustomers].ID;
        noOfCustomers = noOfCustomers + 1;
        writeCustomerFile(-1);
    } else{
        printf("ERROR CODE: 228\n");
    }
    printf("Enter the total number of medicine: ");
    bd->quantityOfMedicine=inputNumber(2,"total number of Medicine");
    for(int i=0;i<bd->quantityOfMedicine;i++){
        again9:
        printf("Enter the medicine ID: ");
        ID=inputNumber(6,"ID");
        flagIndex=checkMedicine(ID);
        float tempAmt=0.0f;
        if (flagIndex >= 0)
        {
            strcpy(bd->medicine[i].name, medicineArr[flagIndex].name);
            bd->medicine[i].mfgDate.dd = medicineArr[flagIndex].mfgDate.dd;
            bd->medicine[i].mfgDate.mm = medicineArr[flagIndex].mfgDate.mm;
            bd->medicine[i].mfgDate.yyyy = medicineArr[flagIndex].mfgDate.yyyy;
            bd->medicine[i].expDate.dd = medicineArr[flagIndex].expDate.dd;
            bd->medicine[i].expDate.mm = medicineArr[flagIndex].expDate.mm;
            bd->medicine[i].expDate.yyyy = medicineArr[flagIndex].expDate.yyyy;
            bd->medicine[i].amount = medicineArr[flagIndex].amount;
            bd->medicine[i].ID = medicineArr[flagIndex].ID;
            tempAmt = tempAmt + (medicineArr[flagIndex].amount);
        }
        else
        {
            printf("\n%d is a Wrong Medicine ID, Plese choose from the list\n",ID);
            viewMedicineDetails();
            printf("\n");
            goto again9;
        }
        again19:
        printf("Enter the quantity of medicineId (%d): ",ID);
        bd->qtyMed[i]=inputNumber(2,"quantity");
        if(bd->qtyMed[i]==0){
            printf("\nQuantity cant be 0\n");
            goto again19;
        }
        totalAmt=totalAmt+(tempAmt*bd->qtyMed[i]);
    }
    time_t t=time(NULL);
    struct tm tm = *localtime(&t);
    bd->date.dd=tm.tm_mday;
    bd->date.mm=tm.tm_mon+1;
    bd->date.yyyy=tm.tm_year+1900;
    printf("Billing date: %02d.%02d.%d\n",bd->date.dd,bd->date.mm,bd->date.yyyy);

    printf("Enter the tax%%: ");
    bd->tax=inputNumber(2,"tax");
    bd->tax = (totalAmt * bd->tax) / 100;
    bd->totalAmount = totalAmt + bd->tax;
    again10:
    printf("Enter the new bill ID (Max 6 digits): ");
    bd->ID=inputNumber(6,"ID");
    if (checkBill(bd->ID) >= 0)
    {
        printf("Bill of same ID alredy present,\nPlease enter a different ID,\n");
        goto again10;
    }
    printf("\nTotal Bill amount: %.2f\n",bd->totalAmount);
}
void inputStock(struct StockDetails *sd,int ID){
    int ch,qty,flagIndex=checkStock(ID);
    if(flagIndex>=0){
        printf("Old quantity: %d\n", sd->quantity);
        printf("\t1. Increase quantity\n\t2. Decrease quantity\nEnter your choice: ");
        ch=inputChoice(2);
        if(ch==1)
        {
        again10:
            printf("Enter the quantity to add: ");
            qty=inputNumber(9,"qty");
            if (qty < 0)
            {
                printf("New stock cant be negative!\n");
                goto again10;
            }
            sd->quantity = sd->quantity + qty;
        } else if(ch==2){
        again11:
            printf("Enter the quantity to remove: ");
            qty=inputNumber(9,"qty");
            if ((sd->quantity - qty) < 0)
            {
                printf("Stock quantity cant go negative,\nR");
                goto again11;
            }
            else
                sd->quantity = sd->quantity - qty;
        } else
            printf("ERROR at line 233");
    }else{
        sd->medicineID = ID;
    again12:
        printf("Old quantity: %d\n", sd->quantity);
        printf("Enter the quantity to add: ");
        qty=inputNumber(9,"qty");
        if (qty < 0)
        {
            printf("New stock cant be negative!\n");
            goto again12;
        }
        sd->quantity = sd->quantity + qty;
    }
    printf("New updated quantity of MedicineID: %d is %d\n", sd->medicineID, sd->quantity);
}
void inputSupplier(struct SupplierDetails *supd){
    int flagIndex,ID,ch;
    int checked[]={0,0,0,0,0,0,0,0,0,0};
    printf("Enter the supplier name: ");
    nameInput(supd->name,"supplier");
    printf("Enter Ph.No.: ");
    numberInput(supd->number,10);
    printf("Enter the quantity of medicines it provied (upto 9 medicine only): ");
    supd->noOfMedProvide=inputNumber(1,"quantity");
    printf("*****Enter the ID of the medicines\n*****If the ID is not present in the system\n*****You can add it on the go!\n");
    for(int i=0;i<supd->noOfMedProvide;i++){
        again13:
        printf("(%d) Medicine ID: ",i+1);
        ID=inputNumber(6,"ID");
        flagIndex=checkMedicine(ID);
        if(flagIndex==-1){
            printf("\nNo MedicineId: %d is present in the system!\n",ID);
            printf("Please choose from the options below\n");
            printf("\t1. Add new medicine.\n\t2. Renter ID\nEnter your choice: ");
            ch=inputChoice(2);
            if(ch==1){
                inputMedicine(&medicineArr[noOfMedicines]);
                strcpy(supd->medName[i],medicineArr[noOfMedicines].name);
                supd->medicineID[i]=medicineArr[noOfMedicines].ID;
                checked[noOfMedicines]=1;
                noOfMedicines=noOfMedicines+1;
                writeMedicineDetails(-1);
                printf("New (%d) Medicine ID: %d\n",i+1,supd->medicineID[i]);
            } else {
                printf("Renter ");
                goto again13;
            }
        } else if(flagIndex>=0 && checked[flagIndex]==1 && (supd->noOfMedProvide-i-1)>0){
            printf("\nMedicineId: %d is alredy entered!\nPease enter rest %d Ids: ",ID,supd->noOfMedProvide-i-1);
            supd->noOfMedProvide=supd->noOfMedProvide-1;
            goto again13;
        } else if(flagIndex>=0 && checked[flagIndex]==1 && (supd->noOfMedProvide-i-1)==0){
            printf("\nMedicineId: %d is alredy entered!\nproceeding to next input!\n",ID);
            supd->noOfMedProvide=supd->noOfMedProvide-1;
        } else{
            strcpy(supd->medName[i],medicineArr[flagIndex].name);
            supd->medicineID[i]=ID;
            checked[flagIndex]=1;
        }
    }
    again14:
    printf("Enter new supplier ID (upto 6 digits): ");
    ID=inputNumber(6,"ID");
    flagIndex=checkSupplier(ID);
    if(flagIndex>=0){
        printf("SupplierId: %d is alredy present!\nPlease ",ID);
        goto again14;
    } else {
        supd->ID=ID;
    }
}

void readCustomerFile()
{
    FILE *file = fopen(customerFileName, "r");
    if (file == NULL)
    {
        printf("No customer record present.\n");
        file = fopen(customerFileName, "w");
        fprintf(file, "%d\n", noOfCustomers);
    }
    else
    {
        fscanf(file, "%d", &noOfCustomers);
        fscanf(file, "%c", &wasteCh);
        if (noOfCustomers == 0)
            printf("No customer record present.\n");
        else
        {
            for (int i = 0; i < noOfCustomers; i++)
            {
                fgets(customerArr[i].name, 20, file);
                fscanf(file, "%c", &wasteCh);
                fgets(customerArr[i].number, 12, file);
                fscanf(file, "%d%d", &customerArr[i].age, &customerArr[i].ID);
                fscanf(file, "%c", &wasteCh);
            }
        }
    }
    fclose(file);
}
void writeCustomerFile(int rm)
{
    FILE *file = fopen(customerFileName, "w");
    if(rm!=-1)
        fprintf(file, "%d\n", noOfCustomers-1);
    else
        fprintf(file, "%d\n", noOfCustomers);
    for (int i = 0; i < noOfCustomers; i++)
    {
        if(i==rm)
            continue;
        fprintf(file, "%-20s%-12s  %d %d\n", customerArr[i].name, customerArr[i].number, customerArr[i].age, customerArr[i].ID);
    }
    fclose(file);
}
void viewCustomerDetails()
{
    system("cls");
    if (noOfCustomers == 0)
        printf("No customer record present.\n");
    else
    {
        printf("************ Customer Details\n");
        printf("%-20s%-12s %s  %s\n", "Name", "Number", "Age", "ID");
        for (int i = 0; i < noOfCustomers; i++)
            printf("%-20s%-12s %d   %06d\n", customerArr[i].name, customerArr[i].number, customerArr[i].age, customerArr[i].ID);
    }
}
void removeCustomerDetails(){
    int ID,flagIndex;
    printf("Enter the ID of that customer: ");
    ID=inputNumber(6,"ID");
    flagIndex=checkCustomer(ID);
    system("cls");
    if(flagIndex>=0){
        writeCustomerFile(flagIndex);
        readCustomerFile();
        printf("Customer ID: %d is removed!\n",ID);
    } else {
        printf("No customer ID: %d found!\nTry again!\n",ID);
    }
}
void editCustomerDetails(){
    int ch,ID,flagIndex;
    viewCustomerDetails();
    if(noOfCustomers==0)
        return;
    again17:
    printf("Enter the Customer ID: ");
    ID=inputNumber(6,"ID");
    flagIndex=checkCustomer(ID);
    if(flagIndex==-1){
        printf("\nInvalid CustomerId!\nR");
        goto again17;
    }
    else{
        system("cls");
        printf("**********Selected CustomerID: %d\n",customerArr[flagIndex].ID);
        printf("Name: %s\nNumber: %s\nAge: %d\n",customerArr[flagIndex].name,customerArr[flagIndex].number,customerArr[flagIndex].age);
    }
    printf("Please choose from the options below\n");
    printf("\t1. Edit name\n\t2. Edit Number\n\t3. Edit Age\n\t4. Back to Customer Section\n\t5. Back to Main menu\n\t6. Exit\nEnter Your choice: ");
    ch=inputChoice(6);
    if(ch==1){
        printf("Enter new name: ");
        nameInput(customerArr[flagIndex].name,"customer");
        system("cls");
        printf("Name Updated!!\n");
        printf("New name: %s\nNumber: %s\nAge: %d\n",customerArr[flagIndex].name,customerArr[flagIndex].number,customerArr[flagIndex].age);
        writeCustomerFile(-1);
    } else if(ch==2){
        printf("Enter new number: ");
        numberInput(customerArr[flagIndex].number,10);
        system("cls");
        printf("Number Updated!!\n");
        printf("Name: %s\nNew number: %s\nAge: %d\n",customerArr[flagIndex].name,customerArr[flagIndex].number,customerArr[flagIndex].age);
        writeCustomerFile(-1);
    } else if(ch==3){
        printf("Enter new age: ");
        customerArr[flagIndex].age=inputNumber(2,"age");
        system("cls");
        printf("Age updated!!\n");
        printf("Name: %s\nNumber: %s\nNew age: %d\n",customerArr[flagIndex].name,customerArr[flagIndex].number,customerArr[flagIndex].age);
        writeCustomerFile(-1);
    } else if(ch==4){
        CustomerMain();
    } else if(ch==5){
        main();
    } else if(ch==6){
        printf("Thank You,\nPlease Visit Again!!");
        printf("\nEnter any key to exit.\n");
        fflush(stdin);
        getch();
        exit(0);
    }
}

void readMedicineDetails()
{
    FILE *file = fopen(medicineFileName, "r");
    if (file == NULL)
    {
        printf("No medicine record present.\n");
        file = fopen(medicineFileName, "w");
        fprintf(file, "%d\n", noOfMedicines);
    }
    else
    {
        fscanf(file, "%d", &noOfMedicines);
        fscanf(file, "%c", &wasteCh);
        if (noOfMedicines == 0)
            printf("No medicine record present.\n");
        else
        {
            for (int i = 0; i < noOfMedicines; i++)
            {
                fgets(medicineArr[i].name, 20, file);
                fscanf(file, "%d%d%d", &medicineArr[i].mfgDate.dd, &medicineArr[i].mfgDate.mm, &medicineArr[i].mfgDate.yyyy);
                fscanf(file, "%d%d%d", &medicineArr[i].expDate.dd, &medicineArr[i].expDate.mm, &medicineArr[i].expDate.yyyy);
                fscanf(file, "%d", &medicineArr[i].amount);
                fscanf(file, "%d", &medicineArr[i].ID);
                fscanf(file, "%c", &wasteCh);
            }
        }
    }
    fclose(file);
}
void writeMedicineDetails(int rm)
{
    FILE *file = fopen(medicineFileName, "w");
    if(rm!=-1)
        fprintf(file, "%d\n", noOfMedicines-1);
    else
        fprintf(file, "%d\n", noOfMedicines);
    for (int i = 0; i < noOfMedicines; i++){
        if(rm==i)
            continue;
        fprintf(file, "%-20s%d %d %d %d %d %d %d %d\n", medicineArr[i].name,
                medicineArr[i].mfgDate.dd, medicineArr[i].mfgDate.mm, medicineArr[i].mfgDate.yyyy,
                medicineArr[i].expDate.dd, medicineArr[i].expDate.mm, medicineArr[i].expDate.yyyy,
                medicineArr[i].amount, medicineArr[i].ID);
    }
    fclose(file);
}
void viewMedicineDetails()
{
    system("cls");
    if (noOfMedicines == 0)
        printf("No medicine record present.\n");
    else
    {
        printf("************ Medicine Details\n");
        printf("%-20s%-10s %-10s    %s\t%s\n", "Name", "Mfg.date", " Exp.date", "  Amount", "ID");
        for (int i = 0; i < noOfMedicines; i++)
            printf("%-20s%02d.%02d.%d  %02d.%02d.%d  %*d\t%d\n", medicineArr[i].name,
                   medicineArr[i].mfgDate.dd, medicineArr[i].mfgDate.mm, medicineArr[i].mfgDate.yyyy,
                   medicineArr[i].expDate.dd, medicineArr[i].expDate.mm, medicineArr[i].expDate.yyyy,
                   9,medicineArr[i].amount, medicineArr[i].ID);
    }
}
void removeMedicineDetails(){
    int ID,flagIndex;
    printf("Enter the MedicineID: ");
    ID=inputNumber(6,"ID");
    flagIndex=checkMedicine(ID);
    system("cls");
    if(flagIndex>=0){
        writeMedicineDetails(flagIndex);
        readMedicineDetails();
        printf("Medicine ID: %d is removed!\n",ID);
    } else {
        printf("No MedicineID: %d found!\nTry again!\n",ID);
    }
}

void readBills()
{
    FILE *file = fopen(billFileName, "r");
    if (file == NULL)
    {
        printf("No bills to show.\n");
        file = fopen(billFileName, "w");
        fprintf(file, "%d\n", noOfBills);
    }
    else
    {
        fscanf(file, "%d", &noOfBills);
        fscanf(file, "%c", &wasteCh);
        if (noOfBills == 0)
            printf("No bills to show.\n");
        else
        {
            for (int i = 0; i < noOfBills; i++)
            {
                fgets(billArr[i].customer.name, 20, file);
                fgets(billArr[i].customer.number, 12, file);
                fscanf(file, "%d", &billArr[i].customer.age);
                fscanf(file, "%d", &billArr[i].customer.ID);
                fscanf(file, "%c", &wasteCh);
                fscanf(file, "%d", &billArr[i].quantityOfMedicine);
                fscanf(file, "%c", &wasteCh);
                for (int j = 0; j < billArr[i].quantityOfMedicine && j < 10; j++)
                {
                    fgets(billArr[i].medicine[j].name, 20, file);
                    fscanf(file, "%d", &billArr[i].medicine[j].mfgDate.dd);
                    fscanf(file, "%d", &billArr[i].medicine[j].mfgDate.mm);
                    fscanf(file, "%d", &billArr[i].medicine[j].mfgDate.yyyy);
                    fscanf(file, "%d", &billArr[i].medicine[j].expDate.dd);
                    fscanf(file, "%d", &billArr[i].medicine[j].expDate.mm);
                    fscanf(file, "%d", &billArr[i].medicine[j].expDate.yyyy);
                    fscanf(file, "%d", &billArr[i].medicine[j].amount);
                    fscanf(file, "%d", &billArr[i].medicine[j].ID);
                    fscanf(file, "%d", &billArr[i].qtyMed[j]);
                    fscanf(file, "%c", &wasteCh);
                }
                fscanf(file, "%d", &billArr[i].date.dd);
                fscanf(file, "%d", &billArr[i].date.mm);
                fscanf(file, "%d", &billArr[i].date.yyyy);
                fscanf(file, "%f", &billArr[i].totalAmount);
                fscanf(file, "%f", &billArr[i].tax);
                fscanf(file, "%d", &billArr[i].ID);
                fscanf(file, "%c", &wasteCh);
            }
        }
    }
    fclose(file);
}
void writeBills(int rm)
{
    FILE *file = fopen(billFileName, "w");
    if(rm!=-1)
        fprintf(file, "%d\n", noOfBills-1);
    else
        fprintf(file, "%d\n", noOfBills);
    for (int i = 0; i < noOfBills; i++)
    {
        if(rm==i)
            continue;
        fprintf(file, "%-19s", billArr[i].customer.name);
        fprintf(file, "%-12s", billArr[i].customer.number);
        fprintf(file, "%d %d\n%d\n", billArr[i].customer.age, billArr[i].customer.ID, billArr[i].quantityOfMedicine);
        for (int j = 0; j < billArr[i].quantityOfMedicine; j++)
        {
            fprintf(file, "%-20s%d %d %d %d %d %d %d %d %d\n", billArr[i].medicine[j].name,
                    billArr[i].medicine[j].mfgDate.dd, billArr[i].medicine[j].mfgDate.mm, billArr[i].medicine[j].mfgDate.yyyy,
                    billArr[i].medicine[j].expDate.dd, billArr[i].medicine[j].expDate.mm, billArr[i].medicine[j].expDate.yyyy,
                    billArr[i].medicine[j].amount, billArr[i].medicine[j].ID,billArr[i].qtyMed[j]);
        }
        fprintf(file, "%d %d %d %.2f %.2f %d\n", billArr[i].date.dd, billArr[i].date.mm, billArr[i].date.yyyy,
                billArr[i].totalAmount, billArr[i].tax, billArr[i].ID);
    }
    fclose(file);
}
void billOfIndex(int i)
{
    printf("\n********************** BILL ID: %06d **********************\n", billArr[i].ID);
    printf("Name:   %sAge: %d\nNumber: %-20sID: %d\n\n", billArr[i].customer.name, billArr[i].customer.age, billArr[i].customer.number, billArr[i].customer.ID);
    printf("%-19s%s   %s   %s %s %s\n", "Name", "Mfg.dt", "  Exp.dt", "        Amt", "    ID","Qty");
    for (int j = 0; j < billArr[i].quantityOfMedicine; j++)
    {
        printf("%s%02d.%02d.%d %02d.%02d.%d %*d %*d %d\n", billArr[i].medicine[j].name,
               billArr[i].medicine[j].mfgDate.dd, billArr[i].medicine[j].mfgDate.mm, billArr[i].medicine[j].mfgDate.yyyy,
               billArr[i].medicine[j].expDate.dd, billArr[i].medicine[j].expDate.mm, billArr[i].medicine[j].expDate.yyyy,9,
               billArr[i].medicine[j].amount,6, billArr[i].medicine[j].ID,billArr[i].qtyMed[j]);
    }
    printf("\nDate of purchase: %02d.%02d.%d\nTax Amount: %.2f\n", billArr[i].date.dd, billArr[i].date.mm, billArr[i].date.yyyy,
           billArr[i].tax);
    printf("Total Amount: %.2f\n", billArr[i].totalAmount);
    printf("*************************************************************\n");
}
void viewBills(int ID)
{
    system("cls");
    if (noOfBills == 0)
        printf("No bills to show.\n");
    else
    {
        printf("************ Bills Details\n");
        int index = -1;
        if (ID < 0)
            for (int i = 0; i < noOfBills; i++)
                billOfIndex(i);
        else
        {
            index = checkBill(ID);
            if (index < 0)
            {
                printf("\nPlease choose the Bill ID\n");
                for (int i = 0; i < noOfBills; i++)
                    printf("Bill ID: %04d Name: %s\n", billArr[i].ID, billArr[i].customer.name);
                printf("Enter the ID: ");
                index=inputNumber(6,"ID");
                if (index > 0)
                    viewBills(index);
            }
            else
                billOfIndex(index);
        }
    }
}
void removeBills(){
    int ID,flagIndex;
    printf("Enter the Bill ID: ");
    ID=inputNumber(6,"ID");
    flagIndex=checkBill(ID);
    system("cls");
    if(flagIndex>=0){
        writeBills(flagIndex);
        readBills();
        printf("Bill ID: %d is removed!\n",ID);
    } else {
        printf("No Bill ID: %d found!\nTry again!\n",ID);
    }
}

void readStock()
{
    FILE *file = fopen(stockFileName, "r");
    if (file == NULL)
    {
        printf("No stock details found.\n");
        file = fopen(stockFileName, "w");
        fprintf(file, "0\n");
    }
    else
    {
        fscanf(file, "%d", &noOfStocks);
        fscanf(file, "%c", &wasteCh);
        if (noOfStocks == 0)
            printf("No stock details found.\n");
        else
        {
            for (int i = 0; i < noOfStocks; i++)
            {
                fscanf(file, "%d%d%c", &stockArr[i].medicineID, &stockArr[i].quantity, &wasteCh);
            }
        }
    }
    fclose(file);
}
void writeStock(int rm)
{
    FILE *file = fopen(stockFileName, "w");
    if(rm!=-1)
        fprintf(file, "%d\n", noOfStocks-1);
    else
        fprintf(file, "%d\n", noOfStocks);
    for (int i = 0; i < noOfStocks; i++){
        if(rm==i)
            continue;
        fprintf(file, "%d %d\n", stockArr[i].medicineID, stockArr[i].quantity);
    }
    fclose(file);
}
void viewStock()
{
    system("cls");
    if (noOfStocks == 0)
        printf("No stock details found.\n");
    else
    {
        printf("************ Stock Details\n");
        printf("SupplierID Quantity\n");
        for (int i = 0; i < noOfStocks; i++)
            printf("%d\t   %d\n", stockArr[i].medicineID, stockArr[i].quantity);
    }
}
void removeStock(){
    int ID,flagIndex;
    printf("Enter the Stock ID: ");
    ID=inputNumber(6,"ID");
    flagIndex=checkStock(ID);
    system("cls");
    if(flagIndex>=0){
        writeStock(flagIndex);
        readStock();
        printf("Stock ID: %d is removed!\n",ID);
    } else {
        printf("No StockID: %d found!\nTry again!\n",ID);
    }
}

void readSupplier(){
    FILE *file=fopen(supplierFileName,"r");
    if(file==NULL){
        printf("No supplier record present.\n");
        file = fopen(supplierFileName, "w");
        fprintf(file, "%d\n", noOfSuppliers);
    } else {
        fscanf(file,"%d",&noOfSuppliers);
        fscanf(file,"%c",&wasteCh);
        if(noOfSuppliers==0)
            printf("No supplier record present.\n");
        else{
            for(int i=0;i<noOfSuppliers;i++){
                fgets(supplierArr[i].name,20,file);
                fscanf(file, "%c", &wasteCh);
                fgets(supplierArr[i].number,12,file);
                fscanf(file,"%c",&wasteCh);
                fscanf(file,"%d",&supplierArr[i].noOfMedProvide);
                fscanf(file,"%d",&supplierArr[i].ID);
                fscanf(file,"%c",&wasteCh);
                for(int j=0;j<supplierArr[i].noOfMedProvide;j++){
                    fgets(supplierArr[i].medName[j],20,file);
                    fscanf(file,"%d",&supplierArr[i].medicineID[j]);
                    fscanf(file,"%c",&wasteCh);
                }
            }
        }
    }
    fclose(file);
}
void writeSupplier(int rm){
    FILE *file=fopen(supplierFileName,"w");
    if(rm!=-1)
        fprintf(file,"%d\n",noOfSuppliers-1);
    else
        fprintf(file,"%d\n",noOfSuppliers);
    for(int i=0;i<noOfSuppliers;i++){
        if(rm==i)
        continue;
        fprintf(file,"%-20s%-12s\n",supplierArr[i].name,supplierArr[i].number);
        fprintf(file,"%d %d\n",supplierArr[i].noOfMedProvide,supplierArr[i].ID);
        for(int j=0;j<supplierArr[i].noOfMedProvide;j++){
            fprintf(file,"%-20s%d\n",supplierArr[i].medName[j],supplierArr[i].medicineID[j]);
        }
    }
    fclose(file);
}
void supplierOfIndex(int i){
    printf("\n************** Supplier ID: %04d **********\n",supplierArr[i].ID);
    printf("Name: %s\nNumber: %s\n",supplierArr[i].name,supplierArr[i].number);
    printf("Total number of medicines suppy: %d\n",supplierArr[i].noOfMedProvide);
    printf("\n%-20sMedicineID\n","Name");
    for(int j=0;j<supplierArr[i].noOfMedProvide;j++)
        printf("%-20s%d\n",supplierArr[i].medName[j],supplierArr[i].medicineID[j]);
    printf("SupplierId: %d\n",supplierArr[i].ID);
    printf("*******************************************\n");
}
void viewSupplier(int ID){
    system("cls");
    if(noOfSuppliers==0)
        printf("No supplier record present.\n");
    else{
        printf("************ Supplier Details\n");
        int index=-1;
        if(ID<0)
            for(int i=0;i<noOfSuppliers;i++)
                supplierOfIndex(i);
        else
        {
            index=checkSupplier(ID);
            if(index<0){
                printf("Please choose the Supplier ID\n");
                for(int i=0;i<noOfSuppliers;i++)
                    printf("SupplierID: %d Name: %s\n",supplierArr[i].ID,supplierArr[i].name);
                printf("Enter the ID: ");
                index=inputNumber(6,"ID");
                if(index>0)
                    viewSupplier(index);
            }
            else
            supplierOfIndex(index);
        }
    }
}
void removeSupplier(){
    int ID,flagIndex;
    printf("Enter the ID of the supplier: ");
    ID=inputNumber(6,"ID");
    flagIndex=checkSupplier(ID);
    system("cls");
    if(flagIndex>=0){
        writeSupplier(flagIndex);
        readSupplier();
        printf("Supplier ID: %d is removed!\n",ID);
    } else {
        printf("No Supplier ID: %d found!\nTry again!\n",ID);
    }
}
void editSupplier(){
    int ch,ID,flagIndex;
    viewSupplier(-1);
    if(noOfSuppliers==0)
        return;
        again18:
    printf("Enter the Supplier ID: ");
    ID=inputNumber(6,"ID");
    flagIndex=checkSupplier(ID);
    if(flagIndex==-1){
        printf("\nInvalid SupplierID!\nR");
        goto again18;
    }else{
        system("cls");
        printf("**********Selected SupplierID: %d\n",supplierArr[flagIndex].ID);
        viewSupplier(ID);
    }
    printf("Please choose from the options below\n");
    printf("\t1. Edit name\n\t2. Edit Number\n\t3. Edit all details\n\t4. Replace supplier\n\t5. Back to main menu\n\t6. Back to Supplier section\n\t7. Exit\nEnter your choice: ");
    ch=inputChoice(7);
    if(ch==1){
        printf("Enter new name: ");
        nameInput(supplierArr[flagIndex].name,"supplier");
        system("cls");
        printf("Name Updated!!\n");
        viewSupplier(supplierArr[flagIndex].ID);
        writeSupplier(-1);
    } else if(ch==2){
        printf("Enter the number: ");
        numberInput(supplierArr[flagIndex].number,10);
        system("cls");
        printf("Number Updated!!\n");
        viewSupplier(supplierArr[flagIndex].ID);
        writeSupplier(-1);
    } else if(ch==3 || ch==4){
        supplierArr[flagIndex].ID=0;
        inputSupplier(&supplierArr[flagIndex]);
        writeSupplier(-1);
        printf("New supplier addedd successfully to the system\n");
    } else if(ch==5){
        main();
    } else if(ch==6){
        SupplierMain();
    } else if(ch==7){
        printf("Thank You,\nPlease Visit Again!!");
        printf("\nEnter any key to exit.\n");
        fflush(stdin);
        getch();
        exit(0);
    }
}

void nameInput(char* name,char str[]){
    again2:
    fflush(stdin);
    gets(name);
    if(strlen(name)>19)
    {
        printf("\nName too long!\nRenter the %s name: ",str);
        goto again2;
    }
    if(strlen(name)<4){
        printf("\nName too short!\nRenter the %s name: ",str);
        goto again2;
    }
}
void numberInput(char* number,int len){
    again3:
    fflush(stdin);
    gets(number);
    int count=0;
    if(strlen(number)!=len){
        printf("\nNumber should be 10 digits!\nRenter the number: ");
        goto again3;
    }
    int flag=1;
    for(int i=0;i<len;i++)
    {
        if(number[i]<'0' || number[i]>'9'){
            flag=0;
            break;
        }
        if(number[i]=='0')
        count++;
    }
    if(flag==0 || count==len){
        printf("\nInvalid input!\nRenter the number: ");
        goto again3;
    }
}
int inputNumber(int len,char str[]){
    char number[10];
    int num;
    again4:
    fflush(stdin);
    gets(number);
    if(strlen(number)>len){
        printf("\n%s too long!\nRenter %s (Max %d digits): ",str,str,len);
        goto again4;
    }
    int ilen=(len>strlen(number))?strlen(number):len;
    int flag=1;
    for(int i=0;i<ilen;i++)
        if(number[i]<'0' || number[i]>'9'){
            flag=0;
            break;
        }
    if(flag==0){
        printf("\nInvalid input!\nRenter the %s (Max %d digits): ",str,len);
        goto again4;
    }
    num=atoi(number);
    if(num==0){
        printf("\nInvalid input!\nRenter the %s (Max %d digits): ",str,len);
        goto again4;
    }
    return num;
}
int inputChoice(int lastCh){
    int ch;
    char check[2];
    again7:
    fflush(stdin);
    gets(check);
    if (check[0] >= '0' && check[0] <= '9' && check[1]=='\0')
        ch = check[0] - '0';
    else
        ch = 100;
    if(ch<=0 || ch>lastCh){
        printf("\nInvalid choice!\nRenter your choice: ");
        goto again7;
    }
    return ch;
}

int checkMedicine(int ID)
{
    for (int i = 0; i < noOfMedicines; i++)
    {
        if (medicineArr[i].ID == ID)
            return i;
    }
    return -1;
}
int checkCustomer(int ID)
{
    for (int i = 0; i < noOfCustomers; i++)
        if (customerArr[i].ID == ID)
            return i;
    return -1;
}
int checkBill(int ID)
{
    for (int i = 0; i < noOfBills; i++)
        if (billArr[i].ID == ID)
            return i;
    return -1;
}
int checkStock(int ID)
{
    for (int i = 0; i < noOfStocks; i++)
        if (stockArr[i].medicineID == ID)
            return i;
    return -1;
}
int checkSupplier(int ID){
    for(int i=0;i<noOfSuppliers;i++)
        if(supplierArr[i].ID == ID)
            return i;
    return -1;
}

void CustomerMain()
{
    int ch;
    system("cls");
    while (1)
    {
        printf("\n************ Medical Store Management System **********\n");
        printf("************ Customer Section\n\n");
        printf("Please choose from the options below\n");
        printf("\t1. Add new customer\n\t2. Remove old customer\n\t3. View customer details\n\t4. Edit customer details\n\t5. Back to main menu\n\t6. Exit\nEnter your choice: ");
        ch=inputChoice(6);
        if (ch == 1)
        {
            system("cls");
            printf("\nAdding new customer:\n");
            inputCustomer(&customerArr[noOfCustomers]);
            noOfCustomers = noOfCustomers + 1;
            writeCustomerFile(-1);
            printf("New customer addedd successfully to the system\n");
        }
        else if (ch == 2)
        {
            system("cls");
            if(noOfCustomers>0){
                viewCustomerDetails();
                removeCustomerDetails();
            } else
                printf("No more customers to remove!\n");
        }
        else if (ch == 3)
            viewCustomerDetails();
        else if (ch == 4)
            editCustomerDetails();
        else if (ch == 5)
            main();
        else if (ch == 6)
        {
            printf("Thank You,\nPlease Visit Again!!");
            printf("\nEnter any key to exit.\n");
            fflush(stdin);
            getch();
            exit(0);
        }
        else
        {
            system("cls");
            printf("\nWrong Choice!!\n");
        }
    }
}
void MedicineMain()
{
    int ch;
    system("cls");
    while (1)
    {
        printf("\n************ Medical Store Management System **********\n");
        printf("************ Medicine Section\n\n");
        printf("Please choose from the options below\n");
        printf("\t1. Add new medicine\n\t2. Remove old medicine\n\t3. View medicine details\n\t4. Back to main menu\n\t5. Exit\nEnter your choice: ");
        ch=inputChoice(5);
        if (ch == 1)
        {
            system("cls");
            printf("\nAdding new medicine:");
            inputMedicine(&medicineArr[noOfMedicines]);
            noOfMedicines = noOfMedicines + 1;
            writeMedicineDetails(-1);
            printf("New medicine addedd successfully to the system\n");
        }
        else if (ch == 2)
        {
            system("cls");
            if(noOfMedicines>0){
                viewMedicineDetails();
                removeMedicineDetails();
            }
            else
                printf("No more medicine to remove!\n");
        }
        else if (ch == 3)
            viewMedicineDetails();
        else if (ch == 4)
            main();
        else if (ch == 5)
        {
            printf("Thank You,\nPlease Visit Again!!");
            printf("\nEnter any key to exit.\n");
            fflush(stdin);
            getch();
            exit(0);
        }
        else
        {
            system("cls");
            printf("\nWrong Choice!!\n");
        }
    }
}
void BillsMain()
{
    int ch;
    system("cls");
    while (1)
    {
        printf("\n************ Medical Store Management System **********\n");
        printf("************ Bills Section\n\n");
        printf("Please choose from the options below\n");
        printf("\t1. Add new bill\n\t2. Remove old bill\n\t3. View bill details\n\t4. Back to main menu\n\t5. Exit\nEnter your choice: ");
        ch=inputChoice(5);
        if (ch == 1)
        {
            system("cls");
            if (noOfMedicines == 0)
                printf("No medicine in the system to create bill.\n");
            else
            {
                printf("\nAdding new bill:\n");
                inputBill(&billArr[noOfBills]);
                noOfBills = noOfBills + 1;
                writeBills(-1);
                printf("New bill addedd successfully to the system\n");
            }
        }
        else if (ch == 2)
        {
            system("cls");
            if(noOfBills>0){
                viewBills(-1);
                removeBills();
            } else
                printf("No more bills to remove!\n");
        }
        else if (ch == 3)
        {
            if(noOfBills>0)
            {
                printf("Please choose from the options below\n");
                printf("\t1. Show all the bills\n\t2. Show specific bill ID\nEnter your choice: ");
                ch=inputChoice(2);
                if (ch == 1)
                    viewBills(-1);
                else
                {
                again9:
                    printf("Enter Bill ID: ");
                    int ID;
                    ID=inputNumber(6,"ID");
                    if (ID >= 0)
                        viewBills(ID);
                    else
                    {
                        printf("ID cannot be negative\n");
                        goto again9;
                    }
                }
            }
            else{
                system("cls");
                printf("No bills to show.\n");
            }
        }
        else if (ch == 4)
            main();
        else if (ch == 5)
        {
            printf("Thank You,\nPlease Visit Again!!");
            printf("\nEnter any key to exit.\n");
            fflush(stdin);
            getch();
            exit(0);
        }
        else
        {
            system("cls");
            printf("\nWrong Choice!!\n");
        }
    }
}
void StockMain()
{
    int ch, ID;
    system("cls");
    while (1)
    {
        printf("\n************ Medical Store Management System **********\n");
        printf("************ Stock Section\n\n");
        printf("Please choose from the options below\n");
        printf("\t1. Add/Update new stock\n\t2. Remove old stock\n\t3. View stock details\n\t4. Back to main menu\n\t5. Exit\nEnter your choice: ");
        ch=inputChoice(5);
        if (ch == 1)
        {
            system("cls");
            if(noOfMedicines==0)
                printf("No medicine stored in the system!\n\tPlease add some.\n");
            else{
                viewMedicineDetails();
                printf("\nPlease make sure that MedicineID is present in the system.\n");
            again16:
                printf("Enter the MedicineID: ");
                ID=inputNumber(6,"ID");
                int flagMedicine = checkMedicine(ID);
                if (flagMedicine == -1)
                {
                    viewMedicineDetails();
                    printf("No MedicineID: %d present in the system!\nPlease select from the list above\n",ID);
                    goto again16;
                }
                int flagIndex = checkStock(ID);
                if (flagIndex >= 0)
                {
                    printf("\nUpdating old stock:\n");
                    inputStock(&stockArr[flagIndex], ID);
                    printf("Stock updated successfully to the system\n");
                }
                else
                {
                    printf("\nAdding new stock:\n");
                    inputStock(&stockArr[noOfStocks], ID);
                    noOfStocks = noOfStocks + 1;
                    printf("Stock added successfully to the system\n");
                }
                writeStock(-1);
            }
        }
        else if (ch == 2)
        {
            system("cls");
            if(noOfStocks>0){
                viewStock();
                removeStock();
            }
            else
                printf("No more stock left to remove!\n");
        }
        else if (ch == 3)
            viewStock();
        else if (ch == 4)
            main();
        else if (ch == 5)
        {
            printf("Thank You,\nPlease Visit Again!!");
            printf("\nEnter any key to exit.\n");
            fflush(stdin);
            getch();
            exit(0);
        }
        else
        {
            system("cls");
            printf("\nWrong Choice!!\n");
        }
    }
}
void SupplierMain(){
    int ch;
    system("cls");
    while(1){
        printf("\n************ Medical Store Management System **********\n");
        printf("************ Supplier Section\n\n");
        printf("Please choose from the options below\n");
        printf("\t1. Add new Supplier\n\t2. Remove old Supplier\n\t3. View Supplier details\n\t4. Edit Supplier details\n\t5. Back to main menu\n\t6. Exit\nEnter your choice: ");
        ch=inputChoice(6);
        if(ch==1){
            system("cls");
            printf("\nAdding new Supplier:\n");
            inputSupplier(&supplierArr[noOfSuppliers]);
            noOfSuppliers=noOfSuppliers+1;
            writeSupplier(-1);
            printf("New supplier addedd successfully to the system\n");
        }
        else if(ch==2){
            system("cls");
            if(noOfSuppliers>0){
                viewSupplier(-1);
                removeSupplier();
            }
            else
                printf("No more supplier to remove!\n");
        }
        else if(ch==3){
            if(noOfSuppliers>0){
                printf("Please choose from the options below\n");
                printf("\t1. Show all the suppliers\n\t2. Show specific supplier ID\nEnter your choice: ");
                ch=inputChoice(2);
                if(ch==1)
                {
                    viewSupplier(-1);
                }
                else
                {
                again15:
                    printf("Enter Supplier ID: ");
                    int ID;
                    ID=inputNumber(6,"ID");
                    if(ID>=0)
                        viewSupplier(ID);
                    else{
                        printf("ID cannot be negative\n");
                        goto again15;
                    }
                }
            }
            else{
                system("cls");
                printf("No Suppliers to show.\n");
            }
        }
        else if(ch==4){
            editSupplier();
        }
        else if(ch==5)
            main();
        else if(ch==6)
        {
            printf("Thank You,\nPlease Visit Again!!");
            printf("\nEnter any key to exit.\n");
            fflush(stdin);
            getch();
            exit(0);
        }
        else
        {
            system("cls");
            printf("\nWrong Choice!!\n");
        }
    }
}
void searchMain(){
    int ch;
    system("cls");
    printf("\n************ Medical Store Management System **********\n");
    printf("************ Search Section\n\n");
    printf("Please choose from the options below\n");
    printf("\t1. Search ID\n\t2. Search name\n\t3. Search number\n\t4. Back to main menu\n\t5. Exit\nEnter your choice: ");
    ch=inputChoice(5);
    if(ch==1)
        searchID();
    else if(ch==2)
        searchName();
    else if(ch==3)
        searchNumber();
    else if(ch==4)
        main();
    else if(ch==5){
        printf("Thank You,\nPlease Visit Again!!");
        printf("\nEnter any key to exit.\n");
        fflush(stdin);
        getch();
        exit(0);
    }
    printf("\nPress any key to go back to main menu...");
    fflush(stdin);
    getch();
    main();
}

void searchID(){
    int ID,i,flag=0;
    printf("Enter the ID for searching: ");
    ID=inputNumber(6,"ID");
    i=checkCustomer(ID);
    system("cls");
    if(i>=0){
        printf("\n************ Customer Details of ID: %d\n",ID);
        printf("%-20s%-12s %s  %s\n", "Name", "Number", "Age", "ID");
        printf("%-20s%-12s %d   %06d\n", 
        customerArr[i].name, 
        customerArr[i].number, 
        customerArr[i].age, 
        customerArr[i].ID);
        flag=1;
    }
    i=checkMedicine(ID);
    if(i>=0){
        printf("\n************ Medicine Details of ID: %d\n",ID);
        printf("%-20s%-10s %-10s    %s\t%s\n", "Name", "Mfg.date", " Exp.date", "  Amount", "ID");
        printf("%-20s%02d.%02d.%d  %02d.%02d.%d  %*d\t%d\n", medicineArr[i].name,
        medicineArr[i].mfgDate.dd, medicineArr[i].mfgDate.mm, medicineArr[i].mfgDate.yyyy,
        medicineArr[i].expDate.dd, medicineArr[i].expDate.mm, medicineArr[i].expDate.yyyy,
        9,medicineArr[i].amount, medicineArr[i].ID);
        flag=1;
    }
    i=checkBill(ID);
    if(i>=0){
        printf("\n************ Bills Details of ID: %d\n",ID);
        billOfIndex(i);
        flag=1;
    }
    i=checkStock(ID);
    if(i>=0){
        printf("\n************ Stock Details of ID\n",ID);
        printf("SupplierID Quantity\n");
        printf("%d\t   %d\n", stockArr[i].medicineID, stockArr[i].quantity);
        flag=1;
    }
    i=checkSupplier(ID);
    if(i>=0){
        printf("\n************ Supplier Details of ID: %d\n",ID);
        supplierOfIndex(i);
        flag=1;
    }
    if(flag==0)
        printf("No search results found!\n");
}
void searchName(){
    int ID,i,flag=0,len,showMessage=0;
    char searchname[20];
    printf("Enter the name to search: ");
    nameInput(searchname,"search");
    len=strlen(searchname);
    if(noOfCustomers>0){
        for(int i=0;i<noOfCustomers;i++){
            if(strncasecmp(customerArr[i].name,searchname,len)==0){
                ID=customerArr[i].ID;
                if(showMessage==0){
                    printf("\n************ Customer Details of ID: %d\n",ID);
                    printf("%-20s%-12s %s  %s\n", "Name", "Number", "Age", "ID");
                    showMessage=1;
                }
                printf("%-20s%-12s %d   %06d\n", 
                customerArr[i].name, 
                customerArr[i].number, 
                customerArr[i].age, 
                customerArr[i].ID);
                flag=1;
            }
        }
    }
    showMessage=0;
    if(noOfMedicines>0){
        for(int i=0;i<noOfMedicines;i++){
            if(strncasecmp(medicineArr[i].name,searchname,len)==0){
                ID=medicineArr[i].ID;
                if(showMessage==0){
                    printf("\n************ Medicine Details of ID: %d\n",ID);
                    printf("%-20s%-10s %-10s    %s\t%s\n", "Name", "Mfg.date", " Exp.date", "  Amount", "ID");
                    showMessage=1;
                }
                printf("%-20s%02d.%02d.%d  %02d.%02d.%d  %*d\t%d\n", medicineArr[i].name,
                medicineArr[i].mfgDate.dd, medicineArr[i].mfgDate.mm, medicineArr[i].mfgDate.yyyy,
                medicineArr[i].expDate.dd, medicineArr[i].expDate.mm, medicineArr[i].expDate.yyyy,
                9,medicineArr[i].amount, medicineArr[i].ID);
                flag=1;
            }
        }
    }
    showMessage=0;
    if(noOfBills>0){
        for(int i=0;i<noOfBills;i++){
            if(strncasecmp(billArr[i].customer.name,searchname,len)==0){
                ID=billArr[i].ID;
                if(showMessage==0){
                    printf("\n************ Bills Details of ID: %d\n",ID);
                }
                billOfIndex(i);
                flag=1;
            }
        }
    }
    showMessage=0;
    if(noOfSuppliers>0){
        for(int i=0;i<noOfSuppliers;i++){
            if(strncasecmp(supplierArr[i].name,searchname,len)==0){
                ID=billArr[i].ID;
                if(showMessage==0){
                    printf("\n************ Supplier Details of ID: %d\n",ID);
                }
                supplierOfIndex(i);
                flag=1;
            }
        }
    }
    if(flag==0)
        printf("No search results found!\n");
}
void searchNumber(){
    int ID,i,flag=0,showMessage=0;
    char numSearch[12];
    printf("Enter the number to search: ");
    numberInput(numSearch,10);
    if(noOfCustomers>0){
        for(int i=0;i<noOfCustomers;i++){
            if(strncasecmp(customerArr[i].number,numSearch,10)==0){
                ID=customerArr[i].ID;
                if(showMessage==0){
                    printf("\n************ Customer Details of ID: %d\n",ID);
                    printf("%-20s%-12s %s  %s\n", "Name", "Number", "Age", "ID");
                    showMessage=1;
                }
                printf("%-20s%-12s %d   %06d\n", 
                customerArr[i].name, 
                customerArr[i].number, 
                customerArr[i].age, 
                customerArr[i].ID);
                flag=1;
            }
        }
    }
    showMessage=0;
    if(noOfBills>0){
        for(int i=0;i<noOfBills;i++){
            if(strncasecmp(billArr[i].customer.number,numSearch,10)==0){
                ID=billArr[i].ID;
                if(showMessage==0){
                    printf("\n************ Bills Details of ID: %d\n",ID);
                }
                billOfIndex(i);
                flag=1;
            }
        }
    }
    showMessage=0;
     if(noOfSuppliers>0){
        for(int i=0;i<noOfSuppliers;i++){
            if(strncasecmp(supplierArr[i].number,numSearch,10)==0){
                ID=billArr[i].ID;
                if(showMessage==0){
                    printf("\n************ Supplier Details of ID: %d\n",ID);
                }
                supplierOfIndex(i);
                flag=1;
            }
        }
    }
    if(flag==0)
        printf("No search results found!\n");
}

int main()
{
    readCustomerFile();
    readMedicineDetails();
    readBills();
    readStock();
    readSupplier();

    int ch;
    system("color 0a");
    system("cls");
    while (1)
    {
        if(start==0){
            printf("************ Instruction ************\n");
            printf("To stop the application press Ctrl+C\n");
            sleep(4);
            start=1;
        }
        system("cls");
        if(noOfCustomers==0 && noOfMedicines==0 && noOfBills==0 && noOfStocks==0 && noOfSuppliers==0)
            printf("********** Welcome! **********\nRight now all the data files are empty\nPlease add some data first!\n");
        printf("\n************ Medical Store Management System **********\n");
        printf("************ Main Menu\n\n");
        printf("Please choose from the options below\n");
        printf("\t1. Customer Section\n\t2. Medicine Section\n\t3. Bills Section\n\t4. Stock Section\n\t5. Supplier Setion\n\t6. Search\n\t7. Exit\nEnter your choice: ");
        ch=inputChoice(7);
        if (ch == 1)
            CustomerMain();
        else if (ch == 2)
            MedicineMain();
        else if (ch == 3)
            BillsMain();
        else if (ch == 4)
            StockMain();
        else if(ch==5)
            SupplierMain();
        else if (ch == 6)
            searchMain();
        else if (ch == 7)
        {
            printf("Thank You,\nPlease Visit Again!!");
            printf("\nEnter any key to exit.\n");
            fflush(stdin);
            getch();
            exit(0);
        }
        else
        {
            system("cls");
            printf("\nWrong Choice!!\n");
        }
    }
    return 0;
}
