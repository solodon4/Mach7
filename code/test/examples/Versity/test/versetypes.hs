data Op  = Op String deriving (Show, Read, Eq)

data TyPrim   = {-- TyChar   | TyCharN   Int
              | TyUChar  | TyUCharN  Int
              | TyShort  | TyShortN  Int
              | TyUShort | TyUShortN Int
              | TyInt    | TyIntN    Int
              | TyUInt   | TyUIntN   Int
              | TyLong   | TyLongN   Int
              | TyULong  | TyULongN  Int
              | TyLLong  | TyLLongN  Int
              | TyULLong | TyULLongN Int
              | TyFloat  | TyFloatN  Int 
              | --} TyDouble {-- | TyDoubleN Int --}
              deriving (Show, Read, Eq)

data Val = ValDouble CDouble {-- | ValDoubleN [CDouble]
         | ValFloat  CFloat  | ValFloatN  [CFloat]
         | ValChar   CChar   | ValCharN   [CChar]
         | ValUChar  CUChar  | ValUCharN  [CUChar]
         | ValShort  CShort  | ValShortN  [CShort]
         | ValUShort CUShort | ValUShortN [CUShort]
         | ValInt    CInt    | ValIntN    [CInt]
         | ValUInt   CUInt   | ValUIntN   [CUInt]
         | ValLong   CLong   | ValLongN   [CLong]
         | ValULong  CULong  | ValULongN  [CULong]
         | ValLLong  CLLong  | ValLLongN  [CLLong]
         | ValULLong CULLong | ValULLongN [CULLong] --}
         deriving (Show, Read, Eq)

data Reg = Ext TyPrim String | Tmp TyPrim Int | Lit Val deriving (Show, Read, Eq)

data Inst
   = InstBin      Op Reg Reg Reg
   | InstUn       Op Reg Reg
   | InstMove     Reg Reg {--
   | InstParLoad  Reg
   | InstParStore Reg
   | InstSeqLoad  Reg
   | InstSeqStore Reg
   | InstBegin    Sec
   | InstEnd --}
   deriving (Show, Read, Eq)
