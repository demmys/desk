public class LazyValue extends Thunk{
    private int intValue;
    private int[] intArrayValue;

    /*
     * constructor
     */
    public LazyValue(int value){
        super();
        intValue = value;
    }
    public LazyValue(int[] value){
        super();
        intArrayValue = value;
    }

    /*
     * getter
     */
    public int getInt(){
        return intValue;
    }
    public int[] getIntArray(){
        return intArrayValue;
    }

    @Override
        public LazyValue eval(){
            return this;
        }
}
