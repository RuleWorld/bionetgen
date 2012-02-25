package bngparser.dataType;

public class MoleculeBond {
	public MoleculeSites getFirst() {
		return first;
	}
	public void setFirst(MoleculeSites first) {
		this.first = first;
	}
	public MoleculeSites getSecond() {
		return second;
	}
	public void setSecond(MoleculeSites second) {
		this.second = second;
	}
	public String getFirstBond() {
		return firstBond;
	}
	public void setFirstBond(String firstBond) {
		this.firstBond = firstBond;
	}
	public String getSecondBond() {
		return secondBond;
	}
	public void setSecondBond(String secondBond) {
		this.secondBond = secondBond;
	}
	MoleculeSites first;
	MoleculeSites second;
	
	String firstBond;
	String secondBond;
	
}
