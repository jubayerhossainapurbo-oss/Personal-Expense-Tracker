/*
    ============================================
    PERSONAL EXPENSE TRACKER
    CSE103: Structured Programming
    ============================================
    Add, Display, Search, Update, Total
    and File Save/Load
    ============================================
*/

#include <stdio.h>
#include <string.h>

#define MAX 100
#define FILENAME "expenses.txt"

// ============================================
// STRUCTURE: one expense record
// ============================================
struct Expense {
    int id;
    char category[20];
    float amount;
};

struct Expense list[MAX];   // array holding all expense records
int count = 0;               // number of expenses currently stored

// ============================================
// FUNCTION DECLARATIONS
// ============================================
void addExpense();
void displayExpense();
void searchCategory();
void updateExpense();
void totalExpense();
void saveFile();
void loadFile();

void clearInputBuffer();
int readValidInt();
float readValidAmount();

// ============================================
// CLEAR THE INPUT BUFFER
// (removes leftover characters after an invalid scanf)
// ============================================
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // nothing to do, just flushing the buffer
    }
}

// ============================================
// KEEP ASKING UNTIL A VALID INTEGER IS ENTERED
// ============================================
int readValidInt() {
    int value;
    while (scanf("%d", &value) != 1) {
        printf("Invalid input! Please enter a number: ");
        clearInputBuffer();
    }
    return value;
}

// ============================================
// KEEP ASKING UNTIL A VALID, NON-NEGATIVE AMOUNT IS ENTERED
// ============================================
float readValidAmount() {
    float amount;
    while (1) {
        if (scanf("%f", &amount) != 1) {
            printf("Invalid amount! Please enter a number: ");
            clearInputBuffer();
            continue;
        }
        if (amount < 0) {
            printf("Amount cannot be negative! Enter again: ");
            continue;
        }
        return amount;
    }
}

// ============================================
// MAIN FUNCTION
// ============================================
int main() {
    int choice;

    loadFile();   // load previously saved data when the program starts

    do {
        printf("\n===== EXPENSE TRACKER =====\n");
        printf("1. Add Expense\n");
        printf("2. Display All Expenses\n");
        printf("3. Search by Category\n");
        printf("4. Update Expense\n");
        printf("5. Show Total Expense\n");
        printf("6. Save & Exit\n");
        printf("Enter choice: ");

        choice = readValidInt();

        if (choice == 1) {
            addExpense();
        }
        else if (choice == 2) {
            displayExpense();
        }
        else if (choice == 3) {
            searchCategory();
        }
        else if (choice == 4) {
            updateExpense();
        }
        else if (choice == 5) {
            totalExpense();
        }
        else if (choice == 6) {
            saveFile();
            printf("Goodbye!\n");
        }
        else {
            printf("Invalid choice! Please choose 1-6.\n");
        }

    } while (choice != 6);

    return 0;
}

// ============================================
// ADD EXPENSE
// ============================================
void addExpense() {
    if (count >= MAX) {
        printf("Expense list is full!\n");
        return;
    }

    list[count].id = count + 1;   // ID is generated automatically

    printf("\n--- Add Expense ---\n");
    printf("Enter category (e.g. Food): ");

    if (scanf("%19s", list[count].category) != 1) {
        printf("Invalid category!\n");
        clearInputBuffer();
        return;
    }

    printf("Enter amount: ");
    list[count].amount = readValidAmount();   // invalid/negative input is rejected here

    count++;
    printf("Expense added successfully!\n");
}

// ============================================
// DISPLAY ALL EXPENSES
// ============================================
void displayExpense() {
    if (count == 0) {
        printf("No expenses yet.\n");
        return;
    }

    printf("\n%-5s %-15s %10s\n", "ID", "Category", "Amount");
    printf("------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-5d %-15s %10.2f\n",
               list[i].id,
               list[i].category,
               list[i].amount);
    }
}

// ============================================
// SEARCH BY CATEGORY
// ============================================
void searchCategory() {
    char category[20];
    int found = 0;

    if (count == 0) {
        printf("No expenses to search.\n");
        return;
    }

    printf("Enter category: ");

    if (scanf("%19s", category) != 1) {
        printf("Invalid category!\n");
        clearInputBuffer();
        return;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].category, category) == 0) {
            printf("\nID: %d", list[i].id);
            printf("\nCategory: %s", list[i].category);
            printf("\nAmount: %.2f\n", list[i].amount);
            found = 1;
        }
    }

    if (found == 0) {
        printf("No expense found in this category.\n");
    }
}

// ============================================
// UPDATE EXPENSE
// ============================================
void updateExpense() {
    int id;
    int found = 0;

    if (count == 0) {
        printf("No expenses to update.\n");
        return;
    }

    printf("Enter ID to update: ");
    id = readValidInt();

    for (int i = 0; i < count; i++) {
        if (list[i].id == id) {
            printf("Enter new category: ");

            if (scanf("%19s", list[i].category) != 1) {
                printf("Invalid category! Update cancelled.\n");
                clearInputBuffer();
                return;
            }

            printf("Enter new amount: ");
            list[i].amount = readValidAmount();

            found = 1;
            printf("Expense updated successfully!\n");
            break;
        }
    }

    if (found == 0) {
        printf("ID not found.\n");
    }
}

// ============================================
// SHOW TOTAL EXPENSE
// ============================================
void totalExpense() {
    float total = 0;

    for (int i = 0; i < count; i++) {
        total = total + list[i].amount;
    }

    printf("\nTotal expenses: %d\n", count);
    printf("Total amount: %.2f\n", total);
}

// ============================================
// SAVE DATA TO FILE
// ============================================
void saveFile() {
    FILE *fp;
    fp = fopen(FILENAME, "w");

    // fopen() returns NULL if it fails (no write permission, disk full, etc.)
    if (fp == NULL) {
        printf("Error! Could not save data.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %.2f\n", list[i].id, list[i].category, list[i].amount);
    }

    fclose(fp);
    printf("Data saved successfully.\n");
}

// ============================================
// LOAD DATA FROM FILE
// ============================================
void loadFile() {
    FILE *fp;
    fp = fopen(FILENAME, "r");

    // If the file doesn't exist, this is just the first run -- not an error,
    // so the program simply starts with an empty list.
    if (fp == NULL) {
        return;
    }

    // A record is only added when all 3 values (id, category, amount) are
    // read successfully. We don't need a separate check for negative amounts
    // here, because saveFile() never writes a negative amount in the first
    // place -- readValidAmount() already blocks it at input time.
    while (count < MAX &&
           fscanf(fp, "%d %19s %f", &list[count].id, list[count].category, &list[count].amount) == 3) {
        count++;
    }

    fclose(fp);
}
