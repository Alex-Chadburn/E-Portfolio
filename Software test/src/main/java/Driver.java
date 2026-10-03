import java.util.Scanner;

public class Driver {

    private static final Scanner scanner = new Scanner(System.in);
    private static final ContactService service = new ContactService();

    public static void main(String[] args) {
        boolean running = true;

        System.out.println("Welcome to the Contact Service!");

        while (running) {
            displayMenu();

            String userChoice = scanner.nextLine().trim().toLowerCase();

            switch (userChoice) {
                case "1":
                    addContact();
                    break;

                case "2":
                    deleteContact();
                    break;

                case "3":
                    updateContact();
                    break;

                case "4":
                    viewContact();
                    break;

                case "q":
                    running = false;
                    System.out.println("Goodbye!");
                    break;

                default:
                    System.out.println("Invalid option. Please choose 1-4 or q.");
            }
        }

        scanner.close();
    }

    private static void displayMenu() {
        System.out.println("\n--- Contact Service Menu ---");
        System.out.println("1. Add contact");
        System.out.println("2. Delete contact");
        System.out.println("3. Update contact");
        System.out.println("4. View contact");
        System.out.println("q. Quit");
        System.out.print("Choose an option: ");
    }

    private static void addContact() {
        System.out.println("\n--- Add Contact ---");

        try {
            System.out.print("Enter contact ID: ");
            String contactID = scanner.nextLine();

            System.out.print("Enter first name: ");
            String firstName = scanner.nextLine();

            System.out.print("Enter last name: ");
            String lastName = scanner.nextLine();

            System.out.print("Enter phone number (10 digits): ");
            String phoneNumber = scanner.nextLine();

            System.out.print("Enter home address: ");
            String homeAddress = scanner.nextLine();

            Contact contact = new Contact(
                    contactID,
                    firstName,
                    lastName,
                    phoneNumber,
                    homeAddress
            );

            if (service.addContact(contact)) {
                System.out.println("Contact added successfully.");
            } else {
                System.out.println("A contact with that ID already exists.");
            }

        } catch (IllegalArgumentException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    private static void deleteContact() {
        System.out.println("\n--- Delete Contact ---");

        System.out.print("Enter contact ID to delete: ");
        String contactID = scanner.nextLine();

        try {
            service.deleteContact(contactID);
            System.out.println("Contact deleted successfully.");
        } catch (IllegalArgumentException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    private static void updateContact() {
        System.out.println("\n--- Update Contact ---");

        System.out.print("Enter contact ID to update: ");
        String contactID = scanner.nextLine();

        if (service.getContact(contactID) == null) {
            System.out.println("Error: Contact ID not found.");
            return;
        }

        System.out.println("What would you like to update?");
        System.out.println("1. First name");
        System.out.println("2. Last name");
        System.out.println("3. Phone number");
        System.out.println("4. Home address");
        System.out.print("Choose an option: ");

        String userChoice = scanner.nextLine();

        try {
            switch (userChoice) {
                case "1":
                    System.out.print("Enter new first name: ");
                    String firstName = scanner.nextLine();
                    service.updateFirstName(contactID, firstName);
                    System.out.println("First name updated successfully.");
                    break;

                case "2":
                    System.out.print("Enter new last name: ");
                    String lastName = scanner.nextLine();
                    service.updateLastName(contactID, lastName);
                    System.out.println("Last name updated successfully.");
                    break;

                case "3":
                    System.out.print("Enter new phone number: ");
                    String phoneNumber = scanner.nextLine();
                    service.updatePhoneNumber(contactID, phoneNumber);
                    System.out.println("Phone number updated successfully.");
                    break;

                case "4":
                    System.out.print("Enter new home address: ");
                    String address = scanner.nextLine();
                    service.updateHomeAddress(contactID, address);
                    System.out.println("Home address updated successfully.");
                    break;

                default:
                    System.out.println("Invalid update option.");
            }

        } catch (IllegalArgumentException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    private static void viewContact() {
        System.out.println("\n--- View Contact ---");

        System.out.print("Enter contact ID: ");
        String contactID = scanner.nextLine();

        Contact contact = service.getContact(contactID);

        if (contact == null) {
            System.out.println("Contact not found.");
            return;
        }

        System.out.println("\nContact Information:");
        System.out.println("ID: " + contact.getContactID());
        System.out.println("First Name: " + contact.getfirstName());
        System.out.println("Last Name: " + contact.getlastName());
        System.out.println("Phone Number: " + contact.getphoneNumber());
        System.out.println("Home Address: " + contact.getAddress());
    }
}