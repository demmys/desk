package org.desklang.primitive;

import org.desklang.Function;

public class Int extends Function{
    int intval;

    public Int(int val){
        super();
        intval = val;
    }

    @Override
        public Int eval(){
            return this;
        }
}
