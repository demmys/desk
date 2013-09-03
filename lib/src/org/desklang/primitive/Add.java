package org.desklang.primitive;

import org.desklang.Function;

/*
 * add :: Int -> Int -> Int
 */
public class Add extends Function{
    public Add(){
        super(2);
    }

    @Override
        public Int eval(){
            int a = draw(0).eval().intval;
            int b = draw(1).eval().intval;
            return new Int(a + b);
        }
}
