#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_EMPLOYEES 100
#define MAX_CUSTOMERS 100
#define MAX_POLICIES 100
#define MAX_CLAIMS 100
#define MAX_USERS 100
#define MAX_BENEFICIARIES 100

// User structure
typedef struct {
    char username[100];
    char password[100];
    int access_level; // 0 for customer, 1 for agent, 2 for admin
} User;

// Database of users
User users[MAX_USERS];
int num_users = 0;

// Function prototypes
void register_user();
bool authenticate(char username[], char password[], int *access_level);

typedef struct {
    int policyNumber;
    char type[50];
    char coverageDetails[100];
    float premiumAmount;
    char startDate[20];
    char endDate[20];
    char beneficiaries[MAX_BENEFICIARIES][50];
    int numBeneficiaries;
} Policy;

typedef struct {
    int claim_id;
    char claim_type[50];
    char date[20];
    char description[100];
    char status[20];
} Claim;

typedef struct {
    char name[50];
    char address[100];
    char phone[15];
    char email[50];
    Policy policies[MAX_POLICIES];
    int policyCount;
    int claimCount;
} Customer;

typedef struct {
    int employeeID;
    char name[50];
    int sales;
    float revenueGenerated;
} Employee;

Customer customers[MAX_CUSTOMERS];
int customerCount = 0;
Policy policies[MAX_POLICIES];
int numPolicies = 0;

// Function Prototypes
void addCustomer();
void updateCustomer();
void viewCustomer();
void customerAccess();
void initiateClaim(Claim *claim);
void reviewClaim(Claim *claim);
void approveClaim(Claim *claim);
void viewStatus(Claim *claim);
void createPolicy();
void viewPolicies();
void searchPolicy();
void updatePolicy();

int main() {
    int choice;
    char username[100];
    char password[100];
    int access_level;

    do {
        printf("\nMain Menu:\n");
        printf("1. Register User\n2. Login\n3. Add New Customer\n");
        printf("4. Update Customer Information\n5. View Customer Profile\n");
        printf("6. Customer Access\n7. Claims Processing System\n8. Policy Management System\n");
        printf("9. Manage Employees\n10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                register_user();
                break;
            case 2:
                printf("Enter username: ");
                scanf("%s", username);
                printf("Enter password: ");
                scanf("%s", password);
                if (authenticate(username, password, &access_level)) {
                    printf("Login successful! Access level: %d\n", access_level);
                } else {
                    printf("Invalid username or password!\n");
                }
                break;
            case 3:
                addCustomer();
                break;
            case 4:
                updateCustomer();
                break;
            case 5:
                viewCustomer();
                break;
            case 6:
                customerAccess();
                break;
            case 7:
                // Claims Processing System
                {
                    Claim claim;
                    int claim_choice;
                    printf("\nClaims Processing System\n");
                    printf("1. Initiate Claim\n");
                    printf("2. Review Claim\n");
                    printf("3. Approve Claim\n");
                    printf("4. View Claim Status\n");
                    printf("5. Exit\n");
                    printf("Enter your choice: ");
                    scanf("%d", &claim_choice);

                    switch (claim_choice) {
                        case 1:
                            initiateClaim(&claim);
                            break;
                        case 2:
                            reviewClaim(&claim);
                            break;
                        case 3:
                            approveClaim(&claim);
                            break;
                        case 4:
                            viewStatus(&claim);
                            break;
                        case 5:
                            printf("Exiting Claims Processing System...\n");
                            break;
                        default:
                            printf("Invalid choice! Please enter a valid option.\n");
                    }
                }
                break;
            case 8:
                // Policy Management System
                {
                    do {
                        // Display menu
                        printf("\nPolicy Management System\n");
                        printf("1. Create Policy\n");
                        printf("2. View Policies\n");
                        printf("3. Search Policy\n");
                        printf("4. Update Policy\n");
                        printf("5. Exit\n");
                        printf("Enter your choice: ");
                        scanf("%d", &choice);

                        // Perform action based on user choice
                        switch(choice) {
                            case 1:
                                createPolicy();
                                break;
                            case 2:
                                viewPolicies();
                                break;
                            case 3:
                                searchPolicy();
                                break;
                            case 4:
                                updatePolicy();
                                break;
                            case 5:
                                printf("Exiting Policy Management System...\n");
                                break;
                            default:
                                printf("Invalid choice. Please enter a number between 1 and 5.\n");
                        }
                    } while(choice != 5);
                }
                break;
            case 9:
                // Manage Employees
                {
                    Employee employees[MAX_EMPLOYEES];
                    int numEmployees;

                    // Ask the user for the number of employees
                    printf("Enter the number of employees: ");
                    scanf("%d", &numEmployees);

                    // Input employee data
                    for (int i = 0; i < numEmployees; i++) {
                        printf("Enter details for Employee %d:\n", i + 1);
                        printf("Employee ID: ");
                        scanf("%d", &employees[i].employeeID);
                        printf("Name: ");
                        scanf("%s", employees[i].name);
                        printf("Sales: ");
                        scanf("%d", &employees[i].sales);
                        printf("Revenue Generated: ");
                        scanf("%f", &employees[i].revenueGenerated);
                    }


                }
                break;
            case 10:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 10);

    return 0;
}
void register_user() {
    FILE *file = fopen("registration.csv", "a");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    User newUser;
    printf("Enter username: ");
    scanf("%s", newUser.username);
    printf("Enter password: ");
    scanf("%s", newUser.password);
    printf("Choose access level (0 for customer, 1 for agent, 2 for admin): ");
    scanf("%d", &newUser.access_level);

    fprintf(file, "%s, %s, %d\n", newUser.username, newUser.password, newUser.access_level);
    fclose(file);

    printf("User registered successfully!\n");
}

// Function to authenticate a user
bool authenticate(char username[], char password[], int *access_level) {
    FILE *file = fopen("registration.csv", "r");
    if (file == NULL) {
        printf("Error opening file for reading!\n");
        return false;
    }

    User user;
    while (fscanf(file, "%s, %s, %d", user.username, user.password, &user.access_level) != EOF) {
        if (strcmp(username, user.username) == 0 && strcmp(password, user.password) == 0) {
            *access_level = user.access_level;
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

// Function Definitions for Customer Management
void addCustomer() {
    FILE *file = fopen("customers.csv", "a");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    Customer newCustomer;
    printf("Enter name: ");
    scanf("%s", newCustomer.name);
    printf("Enter address: ");
    scanf("%s", newCustomer.address);
    printf("Enter phone: ");
    scanf("%s", newCustomer.phone);
    printf("Enter email: ");
    scanf("%s", newCustomer.email);

    fprintf(file, "%s, %s, %s, %s\n", newCustomer.name, newCustomer.address, newCustomer.phone, newCustomer.email);
    fclose(file);

    printf("Customer added successfully!\n");
}
void updateCustomer() {
    char name[50];
    printf("Enter the name of the customer to update: ");
    scanf("%49s", name);

    FILE *file = fopen("customers.csv", "r");
    if (file == NULL) {
        printf("Error opening file for reading!\n");
        return;
    }

    Customer customers[MAX_CUSTOMERS];
    int customerCount = 0;
    bool found = false;

    while (fscanf(file, "%49s %99s %14s %49s", customers[customerCount].name, customers[customerCount].address, customers[customerCount].phone, customers[customerCount].email) != EOF) {
        if (strcmp(customers[customerCount].name, name) == 0) {
            found = true;
            printf("Enter new address: ");
            scanf("%99s", customers[customerCount].address);
            printf("Enter new phone: ");
            scanf("%14s", customers[customerCount].phone);
            printf("Enter new email: ");
            scanf("%49s", customers[customerCount].email);
        }
        customerCount++;
    }
    fclose(file);

    if (found) {
        file = fopen("customers.csv", "w");
        if (file == NULL) {
            printf("Error opening file for writing!\n");
            return;
        }

        for (int i = 0; i < customerCount; i++) {
            fprintf(file, "%s ,%s, %s, %s\n", customers[i].name, customers[i].address, customers[i].phone, customers[i].email);
        }
        fclose(file);
        printf("Customer information updated successfully!\n");
    } else {
        printf("Customer not found!\n");
    }
}

void viewCustomer() {
    char name[50];
    printf("Enter the name of the customer to view: ");
    scanf("%s", name);

    FILE *file = fopen("customers.csv", "r");
    if (file == NULL) {
        printf("Error opening file for reading!\n");
        return;
    }

    Customer customer;
    bool found = false;

    while (fscanf(file, "%s, %s, %s, %s", customer.name, customer.address, customer.phone, customer.email) != EOF) {
        if (strcmp(customer.name, name) == 0) {
            printf("Name: %s\n", customer.name);
            printf("Address: %s\n", customer.address);
            printf("Phone: %s\n", customer.phone);
            printf("Email: %s\n", customer.email);
            found = true;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Customer not found!\n");
    }
}
void customerAccess() {
    char name[50];
    printf("Enter your name: ");
    scanf("%s", name);

    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].name, name) == 0) {
            printf("Name: %s\n", customers[i].name);
            printf("Policies:\n");
            for (int j = 0; j < customers[i].policyCount; j++) {
                printf("  Policy Number: %d\n", customers[i].policies[j].policyNumber);
printf("  Policy Type: %s\n", customers[i].policies[j].type);
printf("  Start Date: %s\n", customers[i].policies[j].startDate);
printf("  End Date: %s\n", customers[i].policies[j].endDate);
            }

        }
    }
    printf("Customer not found!\n");
}
void initiateClaim(Claim *claim) {
    printf("\nEnter claim details:\n");
    printf("Claim ID: ");
    scanf("%d", &claim->claim_id);
    printf("Claim Type: ");
    scanf(" %[^\n]", claim->claim_type);
    printf("Date: ");
    scanf(" %[^\n]", claim->date);
    printf("Description: ");
    scanf(" %[^\n]", claim->description);
    strcpy(claim->status, "Initiated");
    printf("Claim initiated successfully.\n");
}

void reviewClaim(Claim *claim) {
    if (strcmp(claim->status, "Initiated") == 0) {
        printf("\nClaim %d is under review.\n", claim->claim_id);
        strcpy(claim->status, "Under Review");
    } else {
        printf("\nClaim %d cannot be reviewed at this stage.\n", claim->claim_id);
    }
}

void approveClaim(Claim *claim) {
    if (strcmp(claim->status, "Under Review") == 0) {
        printf("\nClaim %d approved.\n", claim->claim_id);
        strcpy(claim->status, "Approved");
    } else {
        printf("\nClaim %d cannot be approved at this stage.\n", claim->claim_id);
    }
}

void viewStatus(Claim *claim) {
    printf("\nClaim ID: %d\n", claim->claim_id);
    printf("Claim Type: %s\n", claim->claim_type);
    printf("Date: %s\n", claim->date);
    printf("Description: %s\n", claim->description);
    printf("Status: %s\n", claim->status);
}
int choice;




// Function to create a new policy
void createPolicy() {
    if (numPolicies >= MAX_POLICIES) {
        printf("Cannot create more policies. Maximum limit reached.\n");
        return;
    }

    Policy newPolicy;
    newPolicy.policyNumber = numPolicies + 1;

    printf("Enter policy type: ");
    scanf("%s", newPolicy.type);

    printf("Enter coverage details: ");
    scanf("%s", newPolicy.coverageDetails);

    printf("Enter premium amount: ");
    scanf("%f", &newPolicy.premiumAmount);

    printf("Enter start date (DD/MM/YYYY): ");
    scanf("%s", newPolicy.startDate);

    printf("Enter end date (DD/MM/YYYY): ");
    scanf("%s", newPolicy.endDate);

    printf("Enter number of beneficiaries: ");
    scanf("%d", &newPolicy.numBeneficiaries);

    printf("Enter beneficiary names:\n");
    for (int i = 0; i < newPolicy.numBeneficiaries; i++) {
        scanf("%s", newPolicy.beneficiaries[i]);
    }

    policies[numPolicies++] = newPolicy;
    printf("Policy created successfully.\n");
}

// Function to view all policies
void viewPolicies() {
    if (numPolicies == 0) {
        printf("No policies found.\n");
        return;
    }

    printf("\nList of Policies:\n");
    for (int i = 0; i < numPolicies; i++) {
        Policy policy = policies[i];
        printf("Policy Number: %d\n", policy.policyNumber);
        printf("Type: %s\n", policy.type);
        printf("Coverage Details: %s\n", policy.coverageDetails);
        printf("Premium Amount: %.2f\n", policy.premiumAmount);
        printf("Start Date: %s\n", policy.startDate);
        printf("End Date: %s\n", policy.endDate);
        printf("Beneficiaries: ");
        for (int j = 0; j < policy.numBeneficiaries; j++) {
            printf("%s ", policy.beneficiaries[j]);
        }
        printf("\n\n");
    }
}

// Function to search for a policy by policy number
void searchPolicy() {
    int policyNumber;
    printf("Enter policy number to search: ");
    scanf("%d", &policyNumber);

    for (int i = 0; i < numPolicies; i++) {
        if (policies[i].policyNumber == policyNumber) {
            printf("Policy found:\n");
            printf("Policy Number: %d\n", policies[i].policyNumber);
            printf("Type: %s\n", policies[i].type);
            printf("Coverage Details: %s\n", policies[i].coverageDetails);
            printf("Premium Amount: %.2f\n", policies[i].premiumAmount);
            printf("Start Date: %s\n", policies[i].startDate);
            printf("End Date: %s\n", policies[i].endDate);
            printf("Beneficiaries: ");
            for (int j = 0; j < policies[i].numBeneficiaries; j++) {
                printf("%s ", policies[i].beneficiaries[j]);
            }
            printf("\n");
            return;
        }
    }

    printf("Policy not found.\n");
}

// Function to update policy information
void updatePolicy() {
    int policyNumber;
    printf("Enter policy number to update: ");
    scanf("%d", &policyNumber);

    for (int i = 0; i < numPolicies; i++) {
        if (policies[i].policyNumber == policyNumber) {
            printf("Enter new premium amount: ");
            scanf("%f", &policies[i].premiumAmount);

            printf("Policy updated successfully.\n");
            return;
        }
    }

    printf("Policy not found.\n");
}
// Function to calculate and generate insights on employee performance
void generateSalesPerformanceReport(Employee employees[], int numEmployees) {
    printf("Sales Performance Report:\n");
    printf("-------------------------\n");
    printf("Employee ID\tName\t\tSales\tRevenue Generated\n");
    for (int i = 0; i < numEmployees; i++) {
        printf("%d\t\t%s\t\t%d\t%.2f\n", employees[i].employeeID, employees[i].name, employees[i].sales, employees[i].revenueGenerated);
    }
    printf("\n");
}

// Function to calculate and generate insights on employee performance
void generatePerformanceInsights(Employee employees[], int numEmployees) {
    int totalSales = 0;
    float totalRevenue = 0;
    float averageRevenuePerSale = 0;

    for (int i = 0; i < numEmployees; i++) {
        totalSales += employees[i].sales;
        totalRevenue += employees[i].revenueGenerated;
    }

    if (totalSales > 0) {
        averageRevenuePerSale = totalRevenue / totalSales;
    }

    printf("Performance Insights:\n");
    printf("---------------------\n");
    printf("Total Sales: %d\n", totalSales);
    printf("Total Revenue Generated: %.2f\n", totalRevenue);
    printf("Average Revenue per Sale: %.2f\n", averageRevenuePerSale);
    printf("\n");
}
