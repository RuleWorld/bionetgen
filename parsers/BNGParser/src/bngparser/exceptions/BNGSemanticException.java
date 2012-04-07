package bngparser.exceptions;

public class BNGSemanticException extends Exception {

	private String errorMessage;
	private int line;
	/**
	 * 
	 */
	public BNGSemanticException(String errorMessage, int line) {
		this.setErrorMessage(errorMessage);
		this.setLine(line);
		
	}
	public String getErrorMessage() {
		return errorMessage;
	}
	public void setErrorMessage(String errorMessage) {
		this.errorMessage = errorMessage;
	}
	public int getLine() {
		return line;
	}
	public void setLine(int line) {
		this.line = line;
	}
	private static final long serialVersionUID = -1903753223407985640L;

}
