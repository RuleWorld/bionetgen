package bngparser.exceptions;

public class VariableNotFoundException extends Exception {

	public VariableNotFoundException(String string) {
		System.err.println("Variable not found: " + string);
	}

	/**
	 * 
	 */
	private static final long serialVersionUID = -4419212646836342316L;

}
