INSERT INTO cards (front_text_id,back_text_id)
SELECT
     translations.from_text_id
    ,translations.to_text_id
FROM translations
UNION
SELECT
     translations.to_text_id
    ,translations.from_text_id
FROM translations

