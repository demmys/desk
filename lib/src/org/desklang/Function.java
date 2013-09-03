package org.desklang;

import java.util.Arrays;

public abstract class Function implements Cloneable{
    private Function[] args;
    private int ap;

    /*
     * 引数の数を確定し、関数による処理を開始する。
     */
    public Function(){}
    public Function(int argc){
        this.args = new Function[argc];
        ap = 0;
    }

    /*
     * 関数に引数を適用する。
     * 適用時には関数の評価は起こらない。
     */
    public void apply(Function... args){
        for(Function arg: args){
            this.args[ap++] = arg;
        }
    }

    /*
     * 関数の引数を取り出す。
     */
    public Function draw(int index){
        return args[index];
    }

    /*
     * 関数を評価する。
     * この関数を引数としてアクションが実行されるときに初めて関数は評価される。
     */
    public abstract Function eval();

    @Override
        public Function clone(){
            try{
                Function c = (Function)super.clone();
                this.args = Arrays.copyOf(c.args, c.args.length);
                return c;
            } catch(CloneNotSupportedException e){
                throw new InternalError(e.toString());
            }
        }
}
