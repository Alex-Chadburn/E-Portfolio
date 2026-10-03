import java.util.HashMap;

public class ContactService {
    private HashMap<String, Contact> contactMap;

    public ContactService() {
        contactMap = new HashMap<>();
    }

    // Add a Contact — ensures unique ID
    public boolean addContact(Contact contact) { //bool so we can inform users the contact already exists
        String contactID = contact.getContactID();
        boolean success = false;
        if (contactMap.containsKey(contactID)) {
            return success; // ID already exists, reject
        }
        contactMap.put(contactID, contact);
        success = true;
        return success;
    }

    // Delete Contact by ID
    public void deleteContact(String contactID) {
        if (!contactMap.containsKey(contactID)) {
            throw new IllegalArgumentException("Contact ID not found");
        }
        contactMap.remove(contactID);
    }


    // Update first name
    public void updateFirstName(String contactID, String newFirstName) {
        Contact contact = contactMap.get(contactID);
        if (contact != null) {
            contact.setfirstName(newFirstName);
        }
    }

    // Update last name
    public void updateLastName(String contactID, String newLastName) {
        Contact contact = contactMap.get(contactID);
        if (contact != null) {
            contact.setlastName(newLastName);
        }
    }

    // Update phone number
    public void updatePhoneNumber(String contactID, String newPhoneNumber) {
        Contact contact = contactMap.get(contactID);
        if (contact != null) {
            contact.setphoneNumber(newPhoneNumber);
        }
    }

    // Update address
    public void updateHomeAddress(String contactID, String newAddress) {
        Contact contact = contactMap.get(contactID);
        if (contact != null) {
            contact.setHomeAddress(newAddress);
        }
    }
    public Contact getContact(String contactID) {
        return contactMap.get(contactID);
    }

}
