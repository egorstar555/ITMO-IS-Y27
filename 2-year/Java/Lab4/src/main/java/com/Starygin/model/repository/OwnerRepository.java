package com.Starygin.model.repository;

import com.Starygin.model.entity.Owner;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.querydsl.QuerydslPredicateExecutor;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface OwnerRepository extends JpaRepository<Owner, Long>, QuerydslPredicateExecutor<Owner> {
    List<Owner> findByName(String ownerName);
}
