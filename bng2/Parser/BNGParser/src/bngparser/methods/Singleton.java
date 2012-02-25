package bngparser.methods;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import bngparser.dataType.Register;

public class Singleton {
    public static List<String> variableName = new ArrayList<String>();
    public static boolean conserveNewMolecules = false;
    public static Map<String,Register> memory = new HashMap<String,Register>();
}
