SELECT
     cards.card_id
    ,front.text AS front
    ,back.text AS back
    ,cards.box_id
    ,cards.box_in_date
    ,boxes.duration
    ,datetime(cards.box_in_date,boxes.duration) AS box_out_date
    ,datetime('now') AS now
    ,datetime('now') > datetime(cards.box_in_date,boxes.duration) AS show
    ,random() AS rnd
FROM cards
INNER JOIN texts AS front ON front.text_id = cards.front_text_id
INNER JOIN texts AS back ON back.text_id = cards.back_text_id
INNER JOIN boxes ON boxes.box_id = cards.box_id 
WHERE cards.box_id >= 0
AND datetime('now') >= datetime(cards.box_in_date,boxes.duration)
ORDER BY cards.box_id, cards.box_in_date



