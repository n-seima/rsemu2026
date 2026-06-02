IF OBJECT_ID('dbo.sp_red_CIG_BUY_test', 'P') IS NOT NULL
    DROP PROCEDURE dbo.sp_red_CIG_BUY_test
GO

CREATE PROCEDURE dbo.sp_red_CIG_BUY_test
(
    @SID varchar(64),
    @ID varchar(20),
    @CN varchar(64),
    @JOB int,
    @LEV int,
    @ITMNO int,
    @ITCNT int,
    @CRCNT int,
    @CODE varchar(64),
    @ISRECTOWEB int,
    @RWP int OUTPUT,
    @ECD int OUTPUT
)
AS
BEGIN
    SET NOCOUNT ON

    DECLARE @CP int
    DECLARE @MP int
    DECLARE @UseMP int
    DECLARE @UseCP int

    SET @RWP = 0
    SET @ECD = 104

    IF @CRCNT IS NULL OR @CRCNT <= 0
    BEGIN
        SET @ECD = 104
        RETURN
    END

    BEGIN TRANSACTION

    SELECT @CP = ISNULL(cp, 0), @MP = ISNULL(mp, 0)
      FROM dbo.AcTb WITH (UPDLOCK, ROWLOCK)
     WHERE uid = @ID

    IF @CP IS NULL AND @MP IS NULL
    BEGIN
        SET @ECD = 100
        ROLLBACK TRANSACTION
        RETURN
    END

    IF @CP + @MP < @CRCNT
    BEGIN
        SET @RWP = @CP + @MP
        SET @ECD = 101
        ROLLBACK TRANSACTION
        RETURN
    END

    IF @MP >= @CRCNT
    BEGIN
        SET @UseMP = @CRCNT
        SET @UseCP = 0
    END
    ELSE
    BEGIN
        SET @UseMP = @MP
        SET @UseCP = @CRCNT - @MP
    END

    UPDATE dbo.AcTb
       SET mp = ISNULL(mp, 0) - @UseMP,
           cp = ISNULL(cp, 0) - @UseCP
     WHERE uid = @ID

    SELECT @CP = ISNULL(cp, 0), @MP = ISNULL(mp, 0)
      FROM dbo.AcTb
     WHERE uid = @ID

    SET @RWP = @CP + @MP
    SET @ECD = 0

    INSERT INTO dbo.Point_Log (uid, pdate, cp, mp, code, descr, etc)
    VALUES
    (
        @ID,
        CAST(GETDATE() AS smalldatetime),
        -@UseCP,
        -@UseMP,
        ISNULL(@CODE, ''),
        CAST(@ITMNO AS varchar(32)),
        'RS_CARROT_BUY'
    )

    COMMIT TRANSACTION
END
GO
