USE `poseidondb` ;

-- -----------------------------------------------------
-- Grant rights to poseidonUSER
-- -----------------------------------------------------
GRANT ALL PRIVILEGES ON poseidondb.* TO poseidonUSER@localhost ;
GRANT USAGE ON poseidondb.* to poseidonUSER@localhost IDENTIFIED BY 'poseidon';
