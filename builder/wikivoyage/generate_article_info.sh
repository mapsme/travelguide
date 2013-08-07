#!/bin/bash

set -e -u -x

# Do not create article_info.txt now so that if we fail make will not consider the job done.
rm article_info.tmp || true

for REGION in United_Kingdom Ireland Italy Switzerland Liechtenstein Belarus Spain Portugal France Germany
do
    # Create an empty table.
    $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="DROP TABLE IF EXISTS $REGION"
    $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="CREATE TABLE $REGION \
        (id int(10) unsigned, gen int(3), id1 int(10), title1 varbinary(255), id2 int(10), title2 varbinary(255), primary key(id))"

    # Insert the seed category.
    $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="INSERT INTO $REGION(id, gen) \
        SELECT page_id, 0 FROM page WHERE page_title = '$REGION';"

    # In a loop insert all children (both pages and categories).
    for i in `seq 1 10`
    do
        $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="INSERT INTO $REGION(id, gen, id1, title1) \
            SELECT cl_from, $i, page_id, page_title \
            FROM categorylinks \
            JOIN page ON cl_to = page_title \
            JOIN $REGION ON page_id = id \
            WHERE cl_from NOT IN (SELECT id from $REGION) GROUP BY cl_from"
    done

    # Update id1 (id of parent).
    $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="UPDATE $REGION r1 \
        JOIN page ON r1.title1 = page_title AND page_namespace = 0 \
        JOIN $REGION r2 ON page_id = r2.id \
        SET r1.id1 = r2.id"

    # Update id2 and title2 (for grandparents).
    $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="UPDATE $REGION r1 \
        JOIN $REGION r2 ON r1.id1 = r2.id \
        SET r1.id2 = r2.id1, r1.title2 = r2.title1"

    # Output the final result.
    $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="SELECT \
        page_id, page_title, page_len, image.pp_value, id1, title1, id2, title2 \
        FROM $REGION \
        JOIN page ON page_id = id \
        LEFT JOIN page_props image ON image.pp_page = id and image.pp_propname = 'page_image' \
        WHERE page_namespace = 0 AND page_is_redirect = 0 \
        ORDER BY page_title" \
        --skip-column-names > $REGION.info.txt

    REDIRECT_TABLE=$REGION"_redirect"
    $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="DROP TABLE IF EXISTS $REDIRECT_TABLE"
    $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="CREATE TABLE $REDIRECT_TABLE \
        (from_id int(10), from_title varbinary(255), to_title varbinary(255))"

    # Add direct redirects.
    $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="INSERT INTO $REDIRECT_TABLE \
        SELECT page_id, page_title, rd_title \
        FROM redirect \
        JOIN page ON rd_from = page_id \
        WHERE rd_title IN \
        (SELECT page_title FROM page JOIN $REGION ON page_id = id)"

    # Add double and triple redirects
    for i in `seq 1 5`
    do
        $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="INSERT INTO $REDIRECT_TABLE \
            SELECT page_id, page_title, rd_title \
            FROM redirect \
            JOIN $REDIRECT_TABLE ON rd_title = from_title \
            JOIN page ON rd_from = page_id"
    done

    # Output the final result.
    $MYSQL_BINARY --user=$MYSQL_USER --database=$MYSQL_DATABASE --execute="SELECT \
        from_id, from_title, page_id, to_title \
        FROM $REDIRECT_TABLE \
        JOIN page ON page_title = to_title AND page_namespace = 0 AND page_is_redirect = 0
        JOIN $REGION ON id = page_id
        ORDER BY from_title" \
        --skip-column-names > $REGION.redirect.txt

    echo $REGION >> countries.tmp
done

# Now we are done. Create the final file.
mv countries.tmp countries.txt
