import java.util.Arrays;

public abstract class Thunk implements Cloneable{
    Thunk[] args;
    private int ap;

    public Thunk(){}
    public Thunk(int argc, Thunk[] args){
        this.args = new Thunk[argc];
        ap = 0;
        apply(args);
    }

    public void apply(Thunk... args){
        for(Thunk arg: args){
            this.args[ap++] = arg;
        }
    }

    public abstract LazyValue eval();

    @Override
        public Thunk clone(){
            try{
                Thunk c = (Thunk)super.clone();
                this.args = Arrays.copyOf(c.args, c.args.length);
                return c;
            } catch(CloneNotSupportedException e){
                throw new InternalError(e.toString());
            }
        }

    /*
     * array :: [a]
     */
    public static class Array extends Thunk{
        public Array(int length, Thunk... args){
            super(length, args);
        }

        public Thunk[] get(){
            return args;
        }

        @Override
            public LazyValue eval(){
                int[] values = new int[args.length];
                for(int i = 0; i < args.length; i++){
                    values[i] = args[i].eval().getInt();
                }
                return new LazyValue(values);
            }
    }
}
