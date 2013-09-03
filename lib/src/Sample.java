import org.desklang.Function;
import org.desklang.primitive.*;
import org.desklang.action.*;

public class Sample{
    public static void main(String[] args) {
        Function f = new Add();
        f.apply(new Int(1), new Int(2));
        IO a = new Print();
        a.apply(f);
    }
}
