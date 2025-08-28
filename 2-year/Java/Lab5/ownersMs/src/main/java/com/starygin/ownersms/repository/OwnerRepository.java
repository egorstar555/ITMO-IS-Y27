package com.starygin.ownersms.repository;


import com.starygin.ownersms.entity.Owner;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface OwnerRepository extends JpaRepository<Owner, Long> {
    List<Owner> findByName(String ownerName);

    boolean findByUserId(Long userId);
}
