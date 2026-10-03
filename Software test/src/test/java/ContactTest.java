import org.junit.Before;
import org.junit.Test;
import static org.junit.Assert.*;

public class ContactTest {

    private Contact contact;

    @Before
    public void setUp() {
        contact = new Contact("001", "John", "Doe", "1234567890", "123 Elm St");
    }

    @Test
    public void testValidContactCreation() {
        assertEquals("001", contact.getContactID());
        assertEquals("John", contact.getfirstName());
        assertEquals("Doe", contact.getlastName());
        assertEquals("1234567890", contact.getphoneNumber());
        assertEquals("123 Elm St", contact.getAddress());
    }

    @Test(expected = IllegalArgumentException.class)
    public void testInvalidContactIDTooLong() {
        new Contact("12345678910", "John", "Doe", "1234567890", "123 Elm St");
    }

    @Test(expected = IllegalArgumentException.class)
    public void testInvalidFirstNameTooLong() {
        new Contact("123", "SuperLongName", "Smith", "1234567890", "123 Elm St");
    }

    @Test(expected = IllegalArgumentException.class)
    public void testPhoneLengthIncorrect() {
        new Contact("001", "John", "Doe", "12345", "123 Elm St");
    }

    @Test
    public void testSetLastNameValidUpdate() {
        contact.setlastName("Johnson");
        assertEquals("Johnson", contact.getlastName());
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSetAddressTooLong() {
        contact.setHomeAddress("This address is far longer than thirty characters!");
    }

    @Test
    public void testSetFirstNameValidUpdate() {
        contact.setfirstName("Jane");
        assertEquals("Jane", contact.getfirstName());
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSetFirstNameNull() {
        contact.setfirstName(null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSetFirstNameTooLong() {
        contact.setfirstName("ThisNameIsTooLong");
    }

    @Test
    public void testSetPhoneNumberValidUpdate() {
        contact.setphoneNumber("0987654321");
        assertEquals("0987654321", contact.getphoneNumber());
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSetPhoneNumberNull() {
        contact.setphoneNumber(null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void testSetPhoneNumberInvalidFormat() {
        contact.setphoneNumber("abc123xyz");
    }

    @Test
    public void testBoundaryValues() {
        Contact boundaryContact = new Contact("1234567890", "Firstname1", "Lastname1", "1234567890", "123456789012345678901234567890");
        assertEquals("1234567890", boundaryContact.getContactID());
        assertEquals("Firstname1", boundaryContact.getfirstName());
        assertEquals("Lastname1", boundaryContact.getlastName());
        assertEquals("1234567890", boundaryContact.getphoneNumber());
        assertEquals("123456789012345678901234567890", boundaryContact.getAddress());
    }
}
