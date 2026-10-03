public class Contact{ 
private String contactID;
private String firstName;
private String lastName;
private String phoneNumber;
private String homeAddress;

public Contact(String contactID, String firstName, String lastName, String phone, String homeAddress) {
if(contactID == null || contactID.length() > 10) {
	throw new IllegalArgumentException("Invalid ID");
}
if(firstName == null || firstName.length() > 10) {
	throw new IllegalArgumentException("Invalid first name");
}
if(lastName == null || lastName.length() > 10) {
	throw new IllegalArgumentException("Invalid last name");
}
if(phone == null || !phone.matches("\\d{10}")) { //regex to match for 10 digits
	throw new IllegalArgumentException("Invalid phone number");
}

if(homeAddress == null || homeAddress.length() > 30) {
	throw new IllegalArgumentException("Invalid home address");
}

this.contactID = contactID;
this.firstName = firstName;
this.lastName = lastName;
this.phoneNumber = phone;
this.homeAddress = homeAddress;
}

public String getContactID() {return contactID;}
public String getfirstName() {return firstName;}
public String getlastName() {return lastName;}
public String getphoneNumber() {return phoneNumber;}
public String getAddress() {return homeAddress;}

public void setfirstName(String firstName) {
	if(firstName == null || firstName.length() > 10) {
		throw new IllegalArgumentException("Invalid first name");
	}
	this.firstName = firstName;
}
public void setlastName(String lastName) {
	if(lastName == null || lastName.length() > 10) {
		throw new IllegalArgumentException("Invalid last name");
	}
	this.lastName = lastName;
}
public void setphoneNumber(String phone) {
	if(phone == null || !phone.matches("\\d{10}")) {
		throw new IllegalArgumentException("Invalid phone number");
	}
	this.phoneNumber = phone;
}
public void setHomeAddress(String homeAddress) {
	if(homeAddress == null || homeAddress.length() > 30) {
		throw new IllegalArgumentException("Invalid home address");
	}
	this.homeAddress = homeAddress;
}
} 
