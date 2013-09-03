package org.desklang.action;

import org.desklang.Function;
import org.desklang.primitive.Int;

/*
 * print :: Show a => a -> IO ()
 */
public class Print extends Function implements IO{
    public Print(){
        super(1);
    }

    public Function action(){
        System.out.println(eval().intval);
    }

    @Override
        public Int eval(){
            return draw(0).eval();
        }
}
