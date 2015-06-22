package bngparser.dataType;

import java.util.HashSet;
import java.util.Set;

import org.antlr.runtime.*;

public class ChangeableChannelTokenStream extends CommonTokenStream
{

	Set<Integer> channels;
    public ChangeableChannelTokenStream(TokenSource source) {
        super(source);
        channels= new HashSet<Integer>();
        channels.add(Token.DEFAULT_CHANNEL);
    }

    public Token nextElement() {
        Token t = null;
        while(true) {
            t = super.tokenSource.nextToken();
            t.setTokenIndex(t.getTokenIndex()+1);
            //t.setTokenIndex(tokenIndex++);
            System.out.println(t.getTokenIndex());
            if(channels.contains(t.getChannel())) break;
        }
        return t;
    }
    
    @Override
    public void consume() {
	     if ( p == -1 ) setup();
	     	p++;
	     	sync(p);
	     while ( !channels.contains(((Token)(tokens.get(p))).getChannel())) {
	         p++;
	         sync(p);
	     }
    }

    public void setChannel(int ch) {
        super.channel = ch;
    }
    
    public void addChannel(int ch){
    	channels.add(ch);
    }
    
    public void delChannel(int ch){
    	channels.remove(ch);
    }
    
   // @Override
	/*public int skipOffTokenChannels(int i) {
    	// TODO Auto-generated method stub
    	return super.skipOffTokenChannels(i);
    }*/
}
