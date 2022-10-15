
#include <stdbool.h>
#include <stddef.h>

struct context
{
    int row;
    int row_count;
    int col;
    int col_count;

    int island_number;

    bool should_inc;
};

static void check_right(char **, struct context *);
static void check_spot(char **, struct context *);

static void context_copy(struct context *ctx_dst, struct context *ctx_src)
{
    ctx_dst->row = ctx_src->row;
    ctx_dst->row_count = ctx_src->row_count;
    ctx_dst->col = ctx_src->col;
    ctx_dst->col_count = ctx_src->col_count;
    ctx_dst->island_number = ctx_src->island_number;
    ctx_dst->should_inc = ctx_src->should_inc;
}

static int count_row(char **world)
{
    int i = 0;
    while (world[i] != NULL)
        i++;
    return i;
}

static int count_col(char **world)
{
    int i = 0;
    while (world[0][i] != '\0')
        i++;
    return i;
}

static void check_right(char **world, struct context *ctx)
{
    struct context ctx_right;

    context_copy(&ctx_right, ctx);

    ctx_right.col++;
    if (ctx_right.col < ctx_right.col_count)
        check_spot(world, &ctx_right);
}

static void check_spot(char **world, struct context *ctx)
{
    const bool is_main = ctx->should_inc;

    if (world[ctx->row][ctx->col] != 'X')
        return;

    world[ctx->row][ctx->col] = '0' + ctx->island_number;
    if (is_main)
        ctx->should_inc = false;

    check_right(world, ctx);

    if (is_main) {
        ctx->island_number++;
        ctx->should_inc = true;
    }
}

static void check_row(char **world, struct context *ctx)
{
    for (ctx->col = 0; ctx->col < ctx->col_count; ctx->col++)
        check_spot(world, ctx);
}

int count_island(char **world)
{
    struct context ctx;

    ctx.col_count = count_col(world);
    ctx.row_count = count_row(world);
    ctx.island_number = 0;
    ctx.should_inc = true;

    for (ctx.row = 0; ctx.row < ctx.row_count; ctx.row++)
        check_row(world, &ctx);

    return ctx.island_number;
}
