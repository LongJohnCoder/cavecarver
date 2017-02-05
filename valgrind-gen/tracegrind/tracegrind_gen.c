#include "tracegrind.h"
#include "distorm.h"

/* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv */
/****************************************************************/



void traverse_expr(LCEnv *mce, IRExpr *e) {

    int i;
    if (!e)
        return;

    switch( e->tag ){

    case Iex_Get:
        break;
    case Iex_GetI:
        break;
    case Iex_RdTmp:
        break;

    case Iex_Qop:
        if (e->Iex.Qop.details) {
            traverse_expr(mce, e->Iex.Qop.details->arg1);
            traverse_expr(mce, e->Iex.Qop.details->arg2);
            traverse_expr(mce, e->Iex.Qop.details->arg3);
            traverse_expr(mce, e->Iex.Qop.details->arg4);
        }
        break;

    case Iex_Triop:
        if (e->Iex.Triop.details) {
            traverse_expr(mce, e->Iex.Triop.details->arg1);
            traverse_expr(mce, e->Iex.Triop.details->arg2);
            traverse_expr(mce, e->Iex.Triop.details->arg3);
        }
        break;

    case Iex_Binop:
        traverse_expr(mce, e->Iex.Binop.arg1);
        traverse_expr(mce, e->Iex.Binop.arg2);
        break;

    case Iex_Unop:
        traverse_expr(mce, e->Iex.Unop.arg);
        break;

    case Iex_Load:
        traverse_expr(mce, e->Iex.Load.addr);
        break;

    case Iex_CCall:
        if (e->Iex.CCall.args) {
            for (i = 0; e->Iex.CCall.args[i]; i++) {
                traverse_expr(mce, e->Iex.CCall.args[i]);
            }
        }
        break;
    case Iex_ITE:
        traverse_expr(mce, e->Iex.ITE.cond);
        traverse_expr(mce, e->Iex.ITE.iftrue);
        traverse_expr(mce, e->Iex.ITE.iffalse);
        break;
    case Iex_Const:
    case Iex_Binder:
    case Iex_VECRET:
    case Iex_BBPTR:
        break;
    default:
        VG_(printf)("\n");
        ppIRExpr(e);
        VG_(printf)("\n");
        VG_(tool_panic)("tr_expr.c: Unhandled expression");
        break;
    }
}


void traverse_stmt(LCEnv *mce, IRStmt* st) {

    switch (st->tag) {
    case Ist_NoOp:
    case Ist_AbiHint:
    case Ist_Put:
    case Ist_PutI:
    case Ist_MBE:
        break;
    case Ist_IMark:
        break;
    case Ist_WrTmp:
        traverse_expr(mce, st->Ist.WrTmp.data) ;
        break;
    case Ist_Store:
        traverse_expr(mce, st->Ist.Store.addr) ;
        traverse_expr(mce, st->Ist.Store.data) ;
        break;
    case Ist_StoreG:
        traverse_expr(mce, st->Ist.StoreG.details->addr) ;
        traverse_expr(mce, st->Ist.StoreG.details->data) ;
        traverse_expr(mce, st->Ist.StoreG.details->guard) ;
        break;
    case Ist_LoadG:
        traverse_expr(mce, st->Ist.LoadG.details->addr) ;
        traverse_expr(mce, st->Ist.LoadG.details->alt) ;
        traverse_expr(mce, st->Ist.LoadG.details->guard) ;
        break;
    case Ist_Dirty:
        break;
    case Ist_CAS:
        break;
    case Ist_LLSC:
        break;
    case Ist_Exit:
        break;
    default:
        break;
    }

}

/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

/*
  Local Variables:
  compile-command:"gcc parse.c -o parse"
  mode:c++
  c-basic-offset:4
  c-file-style:"bsd"
  indent-tabs-mode:nil
  fill-column:99
  End:
*/