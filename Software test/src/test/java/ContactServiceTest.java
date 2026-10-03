import org.junit.Test;
import static org.junit.Assert.*;

public class ContactServiceTest {

    @Test
    public void testAddContactSuccessfully() {
        ContactService service = new ContactService();
        Contact c = new Contact("001", "John", "Doe", "1234567890", "123 Elm St");
        assertTrue(service.addContact(c));
    }

    @Test
    public void testRejectDuplicateID() {
        ContactService service = new ContactService();
        Contact c1 = new Contact("001", "John", "Doe", "1234567890", "123 Elm St");
        Contact c2 = new Contact("001", "Jane", "Smith", "0987654321", "456 Oak St");
        service.addContact(c1);
        assertFalse(service.addContact(c2));
    }

    @Test
    public void testDeleteExistingContact() {
        ContactService service = new ContactService();
        Contact c = new Contact("001", "John", "Doe", "1234567890", "123 Elm St");
        service.addContact(c);
        service.deleteContact("001");
        assertNull(service.getContact("001"));
    }

    @Test(expected = IllegalArgumentException.class)
    public void testDeleteNonexistentContactThrows() {
        ContactService service = new ContactService();
        service.deleteContact("404");
    }

    @Test
    public void testUpdatePhoneNumber() {
        ContactService service = new ContactService();
        Contact c = new Contact("001", "John", "Doe", "1234567890", "123 Elm St");
        service.addContact(c);
        service.updatePhoneNumber("001", "9998887777");
        assertEquals("9998887777", service.getContact("001").getphoneNumber());
    }

    @Test(expected = IllegalArgumentException.class)
    public void testUpdateAddressInvalidLength() {
        ContactService service = new ContactService();
        Contact c = new Contact("001", "John", "Doe", "1234567890", "123 Elm St");
        service.addContact(c);
        service.updateHomeAddress("001", "Imagine having a street address that has a name longer than 30 characters");
    }
    @Test
    public void testUpdateFirstNameSuccessfully() {
        ContactService service = new ContactService();
        Contact c = new Contact("001", "John", "Doe", "1234567890", "123 Elm St");
        service.addContact(c);
        service.updateFirstName("001", "Jane");
        assertEquals("Jane", service.getContact("001").getfirstName());
    }

    @Test
    public void testUpdateLastNameSuccessfully() {
        ContactService service = new ContactService();
        Contact c = new Contact("001", "John", "Doe", "1234567890", "123 Elm St");
        service.addContact(c);
        service.updateLastName("001", "Smith");
        assertEquals("Smith", service.getContact("001").getlastName());
    }

    @Test(expected = IllegalArgumentException.class)
    public void testUpdateFirstNameInvalid() {
        ContactService service = new ContactService();
        Contact c = new Contact("001", "John", "Doe", "1234567890", "123 Elm St");
        service.addContact(c);
        service.updateFirstName("001", "ThisNameIsWayTooLong");
    }

    @Test(expected = IllegalArgumentException.class)
    public void testUpdateLastNameInvalid() {
        ContactService service = new ContactService();
        Contact c = new Contact("001", "John", "Doe", "1234567890", "123 Elm St");
        service.addContact(c);
        service.updateLastName("001", null);
    }
}
